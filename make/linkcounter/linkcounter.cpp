// linkcounter.cpp
//
// WinDirStat - Directory Statistics
// Copyright (C) 2003-2004 Bernhard Seifert
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// Author: bseifert@users.sourceforge.net, bseifert@daccord.net
//
// Last modified: $Date: 2004/11/05 16:53:05 $


// usage: linkcounter <headerfile>
//
// headerfile must contain a line like this:
//  
// #define LINKCOUNT 123
//
// linkcounter will increment that number.

#include "stdafx.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const LPCSTR define= "#define LINKCOUNT ";


int IncrementLinkCount(CString& line);


CWinApp _theApp;

int main(int argc, char *argv[], char *envp[])
{
	VERIFY(AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0));

	if (argc != 2)
	{
		puts("usage: linkcounter <headerfile>");
		return 2;
	}

	CString fileName= argv[1];

	try
	{
		int n= 0;

		CStdioFile file(fileName, CFile::modeReadWrite | CFile::typeText);

		CStringArray contents;
		CString line;
		while (file.ReadString(line))
		{
			if (line.Left(lstrlen(define)) == define)
			{
				n= IncrementLinkCount(line);
			}
			contents.Add(line);
		}

		if (n == 0)
		{
			puts("No linkcount found.");
			return 1;
		}

		file.SeekToBegin();
		file.SetLength(0);

		for (int i=0; i < contents.GetSize(); i++)
		{
			file.WriteString(contents[i] + "\n");
		}

		printf("Linkcount updated to %d.\n", n);
	}
	catch (CException *pe)
	{
		puts(MdGetExceptionMessage(pe));
		pe->Delete();

		return 1;
	}

	return 0;
}

int IncrementLinkCount(CString& line)
{
	CString number= line.Mid(lstrlen(define));
	number.TrimLeft();
	number.TrimRight();

	int n= atoi(number);

	n++;

	line.Format("%s %d", define, n);

	return n;
}

// $Log: linkcounter.cpp,v $
// Revision 1.2  2004/11/05 16:53:05  assarbad
// Added Date and History tag where appropriate.
//
