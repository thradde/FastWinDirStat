
class CDlgCognosProgress : public CDialog
{
	DECLARE_DYNAMIC(CDlgCognosProgress)

protected:
	UINT_PTR m_Timer;

public:
	CDlgCognosProgress(CWnd* pParent = NULL);   // standard constructor

protected:
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};


class CCogFileFind : public CFileFind
{
protected:
	CognosDriveHandle	m_hCogDriveHandle;
	bool				m_bUsesCognosNT;

public:
	CCogFileFind();
	virtual ~CCogFileFind();

public:
	// Attributes
	BOOL IsReparsePoint() const
	{
		return MatchesMask(FILE_ATTRIBUTE_REPARSE_POINT);
	}

	// Operations
	void Close();
	virtual BOOL FindFile(LPCTSTR pstrName = NULL, DWORD dwUnused = 0) override;
	virtual BOOL FindNextFile() override;

	// WinDirStat extensions
	DWORD GetAttributes() const;
	ULONGLONG GetCompressedLength() const;
};

typedef int (WINAPI *TFpCogConnect)(const wchar_t *license_key);
typedef int (WINAPI *TFpCogDisconnect)();
typedef CognosDriveHandle (WINAPI *TFpCogOpenDrive)(wchar_t drive_letter, TCogonosProgressCallback progress_callback, void *cb_userdata);
typedef int (WINAPI *TFpCogCloseDrive)(CognosDriveHandle hDrive);
typedef uint64_t (WINAPI *TFpCogGetMemoryUsage)(CognosDriveHandle hDrive);
typedef CognosFindHandle (WINAPI *TFpCogFindFirstFile)(CognosDriveHandle hDrive, const wchar_t *lpFilePath, COG_FIND_DATA *lpFindFileData);
typedef int (WINAPI *TFpCogFindNextFile)(CognosFindHandle hFindFile, COG_FIND_DATA *lpFindFileData);
typedef void (WINAPI *TFpCogFindClose)(CognosFindHandle hFindFile);

extern HMODULE ghCognosDll;

extern TFpCogConnect	fpCogConnect;
extern TFpCogDisconnect	fpCogDisconnect;

bool LoadCogonosDll();
void FreeCogonosDll();
void CogWdsCloseDrive(const CString &path);
void CogWdsCloseAllDrives();
