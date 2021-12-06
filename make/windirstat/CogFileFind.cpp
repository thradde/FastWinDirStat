
#include "stdafx.h"
#include <atomic>
#include "windirstat.h"

#ifndef UNICODE
	#error "CognosNT build of WinDirStat requires UNICODE"
#endif

// --------------------------------------------------------------------------------------------------------------------------------------------
//																Globals
// --------------------------------------------------------------------------------------------------------------------------------------------
#define MAX_DRIVES		26		// A .. Z
#define UNUSED_HANDLE	-2

HMODULE ghCognosDll = NULL;

TFpCogConnect			fpCogConnect;
TFpCogDisconnect		fpCogDisconnect;
TFpCogOpenDrive			fpCogOpenDrive;
TFpCogCloseDrive		fpCogCloseDrive;
TFpCogGetMemoryUsage	fpCogGetMemoryUsage;
TFpCogFindFirstFile		fpCogFindFirstFile;
TFpCogFindNextFile		fpCogFindNextFile;
TFpCogFindClose			fpCogFindClose;


class CCognosDriveInfo
{
public:
	CognosDriveHandle	m_hDriveHandle;

	CCognosDriveInfo()
		: m_hDriveHandle(UNUSED_HANDLE)
	{
	}
};

static CCognosDriveInfo garDrives[MAX_DRIVES];


// this enum does not belong to the API!
enum EOpenStatus
{
	enOsPending,
	enOsFailure,
	enOsOpen,
};

static std::atomic<int> gnProgressPercent = 0;
static std::atomic<EOpenStatus> genOpenDriveStatus;
static std::atomic<CognosDriveHandle> ghDrive;


// --------------------------------------------------------------------------------------------------------------------------------------------
//																LoadCogonosDll
// --------------------------------------------------------------------------------------------------------------------------------------------
bool LoadCogonosDll()
{
	ghCognosDll = LoadLibrary(L"cognosapi.dll");
	if (!ghCognosDll)
		return false;

	fpCogConnect = (TFpCogConnect)GetProcAddress(ghCognosDll, "CogConnect");
	fpCogDisconnect = (TFpCogDisconnect)GetProcAddress(ghCognosDll, "CogDisconnect");
	fpCogOpenDrive = (TFpCogOpenDrive)GetProcAddress(ghCognosDll, "CogOpenDrive");
	fpCogCloseDrive = (TFpCogCloseDrive)GetProcAddress(ghCognosDll, "CogCloseDrive");
	fpCogGetMemoryUsage = (TFpCogGetMemoryUsage)GetProcAddress(ghCognosDll, "CogGetMemoryUsage");
	fpCogFindFirstFile = (TFpCogFindFirstFile)GetProcAddress(ghCognosDll, "CogFindFirstFile");
	fpCogFindNextFile = (TFpCogFindNextFile)GetProcAddress(ghCognosDll, "CogFindNextFile");
	fpCogFindClose = (TFpCogFindClose)GetProcAddress(ghCognosDll, "CogFindClose");

	bool loaded =	fpCogConnect != NULL &&
					fpCogDisconnect != NULL &&
					fpCogOpenDrive != NULL &&
					fpCogCloseDrive != NULL &&
					fpCogGetMemoryUsage != NULL &&
					fpCogFindFirstFile != NULL &&
					fpCogFindNextFile != NULL &&
					fpCogFindClose != NULL;

	if (!loaded)
	{
		FreeLibrary(ghCognosDll);
		ghCognosDll = NULL;
	}

	return loaded;
}


void FreeCogonosDll()
{
	if (ghCognosDll)
		FreeLibrary(ghCognosDll);
}


// --------------------------------------------------------------------------------------------------------------------------------------------
//																Progress Dialog
// --------------------------------------------------------------------------------------------------------------------------------------------

IMPLEMENT_DYNAMIC(CDlgCognosProgress, CDialog)

CDlgCognosProgress::CDlgCognosProgress(CWnd* pParent)
	: CDialog(IDD_DIALOG_COGNOS_PROGRESS, pParent)
	, m_Timer(0)
{
}


BEGIN_MESSAGE_MAP(CDlgCognosProgress, CDialog)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// disable ESC key by overriding this method and doing nothing
void CDlgCognosProgress::OnCancel()
{
}


BOOL CDlgCognosProgress::OnInitDialog()
{
	CDialog::OnInitDialog();

	CWnd *ProgressBar = GetDlgItem(IDC_PROGRESSBAR);
	ProgressBar->SendMessage(PBM_SETRANGE, 0, MAKELPARAM(0, 100));
	ProgressBar->SendMessage(PBM_SETSTEP, (WPARAM)1, 0);

	m_Timer = SetTimer(1234, 1, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
}


void CDlgCognosProgress::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == m_Timer)
	{
		CWnd *ProgressBar = GetDlgItem(IDC_PROGRESSBAR);
		ProgressBar->SendMessage(PBM_SETPOS, gnProgressPercent, 0);
		if (genOpenDriveStatus != enOsPending)
			EndDialog(1);
	}

	CDialog::OnTimer(nIDEvent);
}


// --------------------------------------------------------------------------------------------------------------------------------------------
//																ProgressCallback()
// --------------------------------------------------------------------------------------------------------------------------------------------
int ProgressCallback(int progress_percent, void *)
{
	gnProgressPercent = progress_percent;
	return true;	// returning false means abort
}


// --------------------------------------------------------------------------------------------------------------------------------------------
//																OpenDriveThread()
// --------------------------------------------------------------------------------------------------------------------------------------------
unsigned __stdcall OpenDriveThread(void *param)
{
	wchar_t drive_letter = *(wchar_t *)param;

	ghDrive = fpCogOpenDrive(drive_letter, ProgressCallback, NULL);

	gnProgressPercent = 100;
	if (ghDrive == INVALID_DRIVE_HANDLE)
		genOpenDriveStatus = enOsFailure;
	else
		genOpenDriveStatus = enOsOpen;

	_endthreadex(0);
	return 0;
}


// --------------------------------------------------------------------------------------------------------------------------------------------
//																CCogFileFind
// --------------------------------------------------------------------------------------------------------------------------------------------
CCogFileFind::CCogFileFind()
	: CFileFind()
	, m_hCogDriveHandle(INVALID_DRIVE_HANDLE)
	, m_bUsesCognosNT(false)
{
}


CCogFileFind::~CCogFileFind()
{
	Close();
}

void CCogFileFind::Close()
{
	if (m_pFoundInfo != NULL)
	{
		delete m_pFoundInfo;
		m_pFoundInfo = NULL;
	}

	if (m_pNextInfo != NULL)
	{
		delete m_pNextInfo;
		m_pNextInfo = NULL;
	}

	if (m_hContext != NULL)
	{
		if (m_bUsesCognosNT)
			fpCogFindClose(m_hContext);
		else if (m_hContext != INVALID_HANDLE_VALUE)
			::FindClose(m_hContext);
		m_hContext = NULL;
	}
}


int GetDriveIndexFromPath(LPCTSTR path)
{
	int drvindex = -1;

	if (path[1] == _T(':') && path[2] == _T('\\'))
	{
		TCHAR drvletter = _totlower(path[0]);

		if (drvletter >= _T('a') && drvletter <= _T('z'))
			drvindex = drvletter - _T('a');
	}

	return drvindex;
}


BOOL CCogFileFind::FindFile(LPCTSTR pstrName /* = NULL */,	DWORD dwUnused /* = 0 */)
{
	if (!ghCognosDll)
		return CFileFind::FindFile(pstrName, dwUnused);

	CString cwd;
	Close();

	if (pstrName == NULL)
	{
		TCHAR *buffer;

		// Get the current working directory:
		if ((buffer = _tgetcwd(NULL, 0)) == NULL)
		{
			::SetLastError(ERROR_BAD_ARGUMENTS);
			return FALSE;
		}

		cwd = buffer;
		free(buffer);

		// append \*.*
		cwd += _T("\\*.*");
		pstrName = cwd;
	}
	else if (pstrName[0] == _T('\\') && pstrName[1] == _T('\\'))
	{
		// UNC path, bail out
		return CFileFind::FindFile(pstrName, dwUnused);
	}
	else if (_tcsstr(pstrName, _T("..\\")))
	{
		// This is a relative path. CognosNT does not handle relative paths.
		// Build absolute path here.
		LPTSTR fullpath = cwd.GetBufferSetLength(_MAX_PATH);
		pstrName = _tfullpath(fullpath, pstrName, _MAX_PATH);

		// passed name isn't a valid path
		if (pstrName == NULL)
		{
			cwd.ReleaseBuffer(0);
			::SetLastError(ERROR_INVALID_NAME);
			return FALSE;
		}
		cwd.ReleaseBuffer(-1);
	}

	if (_tcslen(pstrName) >= (_countof(((WIN32_FIND_DATA*)m_pNextInfo)->cFileName)))
	{
		::SetLastError(ERROR_BAD_ARGUMENTS);
		return FALSE;
	}

	// check, whether the specified path contains a drive letter
	int drvindex = GetDriveIndexFromPath(pstrName);
	if (drvindex >= 0)
	{
		// CognosNT can work with this path
		// if the drive could not be opened before by CognosNT, bail out
		if (garDrives[drvindex].m_hDriveHandle == INVALID_DRIVE_HANDLE)
			return CFileFind::FindFile(pstrName, dwUnused);

		// if the drive yet has not been scanned by CognosNT, do it now
		if (garDrives[drvindex].m_hDriveHandle == UNUSED_HANDLE)
		{
			// scan in a thread
			gnProgressPercent = 0;
			ghDrive = INVALID_DRIVE_HANDLE;
			genOpenDriveStatus = enOsPending;
			HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, OpenDriveThread, (void *)&pstrName[0], 0, NULL);

			// show progress dialog
			CDlgCognosProgress progress;
			progress.DoModal();				// the dialog returns, when scanning has finished

			WaitForSingleObject(hThread, INFINITE);
			CloseHandle(hThread);

			garDrives[drvindex].m_hDriveHandle = ghDrive;
			if (ghDrive == INVALID_DRIVE_HANDLE)
				return CFileFind::FindFile(pstrName, dwUnused);
			m_hCogDriveHandle = ghDrive;
		}

		m_bUsesCognosNT = true;
		m_pNextInfo = new WIN32_FIND_DATA;	// upwards compatible with COG_FIND_DATA
		WIN32_FIND_DATA *pFindData = (WIN32_FIND_DATA *)m_pNextInfo;
		Checked::tcscpy_s(pFindData->cFileName, _countof(pFindData->cFileName), pstrName);

		m_hContext = fpCogFindFirstFile(garDrives[drvindex].m_hDriveHandle, pstrName, (COG_FIND_DATA *)m_pNextInfo);

		if (m_hContext == NULL)
		{
			Close();
			::SetLastError(ERROR_BAD_ARGUMENTS);
			return FALSE;
		}

		LPTSTR pstrRoot = m_strRoot.GetBufferSetLength(_MAX_PATH);
		LPCTSTR pstr = _tfullpath(pstrRoot, pstrName, _MAX_PATH);

		// passed name isn't a valid path but was found by the API
		ASSERT(pstr != NULL);
		if (pstr == NULL)
		{
			m_strRoot.ReleaseBuffer(0);
			Close();
			::SetLastError(ERROR_INVALID_NAME);
			return FALSE;
		}

		// remove possible file name or search mask from path
		TCHAR strDrive[_MAX_DRIVE], strDir[_MAX_DIR];
		Checked::tsplitpath_s(pstrRoot, strDrive, _MAX_DRIVE, strDir, _MAX_DIR, NULL, 0, NULL, 0);
		Checked::tmakepath_s(pstrRoot, _MAX_PATH, strDrive, strDir, NULL, NULL);
		m_strRoot.ReleaseBuffer(-1);

		return TRUE;
	}

	return CFileFind::FindFile(pstrName, dwUnused);
}


BOOL CCogFileFind::FindNextFile()
{
	ASSERT(m_hContext != NULL );

	if (m_hContext == NULL)
		return FALSE;

	if (m_pFoundInfo == NULL)
		m_pFoundInfo = new WIN32_FIND_DATA;

	ASSERT_VALID(this);

	void* pTemp = m_pFoundInfo;
	m_pFoundInfo = m_pNextInfo;
	m_pNextInfo = pTemp;

	if (m_bUsesCognosNT)
		return fpCogFindNextFile(m_hContext, (COG_FIND_DATA *)m_pNextInfo);

	return ::FindNextFile(m_hContext, (LPWIN32_FIND_DATA)m_pNextInfo);
}


void CogWdsCloseDrive(const CString &path)
{
	// check, whether the specified path contains a drive letter
	int drvindex = GetDriveIndexFromPath(path);
	if (drvindex >= 0)
	{
		if (garDrives[drvindex].m_hDriveHandle < 0)		// invalid or unused
			return;

		fpCogCloseDrive(garDrives[drvindex].m_hDriveHandle);
		garDrives[drvindex].m_hDriveHandle = UNUSED_HANDLE;
	}
}


void CogWdsCloseAllDrives()
{
	for (int drvindex = 0; drvindex < MAX_DRIVES; drvindex++)
	{
		if (garDrives[drvindex].m_hDriveHandle >= 0)
		{
			fpCogCloseDrive(garDrives[drvindex].m_hDriveHandle);
			garDrives[drvindex].m_hDriveHandle = UNUSED_HANDLE;
		}
	}
}


// Function to access the file attributes from outside
DWORD CCogFileFind::GetAttributes() const
{
	ASSERT(m_hContext != NULL);
	ASSERT_VALID(this);

	if (m_pFoundInfo != NULL)
		return ((LPWIN32_FIND_DATA)m_pFoundInfo)->dwFileAttributes;
	else
		return INVALID_FILE_ATTRIBUTES;
}


// Wrapper for file size retrieval
// This function tries to return compressed file size whenever possible.
// If the file is not compressed the uncompressed size is being returned.
ULONGLONG CCogFileFind::GetCompressedLength() const
{
	ASSERT(m_hContext != NULL);
	ASSERT_VALID(this);

	if (m_pFoundInfo != NULL && (((LPWIN32_FIND_DATA)m_pFoundInfo)->dwFileAttributes & FILE_ATTRIBUTE_COMPRESSED))
	{
		if (m_bUsesCognosNT)
		{
			ULONGLONG file_size = ((COG_FIND_DATA *)m_pFoundInfo)->nAllocSizeHigh;
			file_size <<= 32;
			file_size += ((COG_FIND_DATA *)m_pFoundInfo)->nAllocSizeLow;
			return file_size;
		}

		// Try to use the NT-specific API
		if (GetApp()->GetComprSizeApi()->IsSupported())
		{
			ULARGE_INTEGER ret;
			ret.LowPart = GetApp()->GetComprSizeApi()->GetCompressedFileSize(GetFilePath(), &ret.HighPart);

			// Check for error
			if (GetLastError() == NO_ERROR)
				return ret.QuadPart;
		}
	}

	// Use the file size already found by the finder object
	return GetLength();
}
