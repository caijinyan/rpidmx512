/*
 * jamplayer.h
 */

#ifndef JAMSTAPL_H_
#define JAMSTAPL_H_

#include "jbiport.h"

extern "C" {
#include "../jbi_22/code/jbiexprt.h"
}

class JamSTAPL {
public:
	JamSTAPL(PROGRAM_PTR pProgram, long nProgramSize) :
			m_pProgram(pProgram), m_nProgramSize(nProgramSize) {
		PlatformInit();
	}

	JBI_RETURN_TYPE CheckCRC(bool bVerbose = false);
	void PrintInfo();

	void ReadIdCode();
	void ReadUsercode();

	void Erase();
	void Verify();
	void Program();

	JBI_RETURN_TYPE GetResult() const {
		return m_nExecResult;
	}

	const char *GetResultString() const;

	int GetExitCode() const {
		return m_nExitCode;
	}

	const char *GetExitCodeString() const;

private:
	void PlatformInit();
	void Execute(const char *pAction);

private:
	PROGRAM_PTR m_pProgram;
	long m_nProgramSize;
	int m_nResetJtag { 1 };
	long m_nErrorAddress;
	int m_nExitCode { -1 };
	int m_FormatVersion { 2 };
	JBI_RETURN_TYPE m_nExecResult { JBIC_ACTION_NOT_FOUND };
};

#endif /* JAMSTAPL_H_ */
