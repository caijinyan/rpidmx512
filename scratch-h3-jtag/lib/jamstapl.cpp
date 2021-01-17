/*
 * jamstapl.cpp
 *
 */

#undef NDEBUG

#include <stdint.h>
#include <stdio.h>
#include <cassert>

#include <jamstapl.h>

extern "C" {
#include "../jbi_22/code/jbiexprt.h"
}

#include "debug.h"

static constexpr const char *error_text[] = {
/* JBIC_SUCCESS            0 */ "success",
/* JBIC_OUT_OF_MEMORY      1 */ "out of memory",
/* JBIC_IO_ERROR           2 */ "file access error",
/* JAMC_SYNTAX_ERROR       3 */ "syntax error",
/* JBIC_UNEXPECTED_END     4 */ "unexpected end of file",
/* JBIC_UNDEFINED_SYMBOL   5 */ "undefined symbol",
/* JAMC_REDEFINED_SYMBOL   6 */ "redefined symbol",
/* JBIC_INTEGER_OVERFLOW   7 */ "integer overflow",
/* JBIC_DIVIDE_BY_ZERO     8 */ "divide by zero",
/* JBIC_CRC_ERROR          9 */ "CRC mismatch",
/* JBIC_INTERNAL_ERROR    10 */ "internal error",
/* JBIC_BOUNDS_ERROR      11 */ "bounds error",
/* JAMC_TYPE_MISMATCH     12 */ "type mismatch",
/* JAMC_ASSIGN_TO_CONST   13 */ "assignment to constant",
/* JAMC_NEXT_UNEXPECTED   14 */ "NEXT unexpected",
/* JAMC_POP_UNEXPECTED    15 */ "POP unexpected",
/* JAMC_RETURN_UNEXPECTED 16 */ "RETURN unexpected",
/* JAMC_ILLEGAL_SYMBOL    17 */ "illegal symbol name",
/* JBIC_VECTOR_MAP_FAILED 18 */ "vector signal name not found",
/* JBIC_USER_ABORT        19 */ "execution cancelled",
/* JBIC_STACK_OVERFLOW    20 */ "stack overflow",
/* JBIC_ILLEGAL_OPCODE    21 */ "illegal instruction code",
/* JAMC_PHASE_ERROR       22 */ "phase error",
/* JAMC_SCOPE_ERROR       23 */ "scope error",
/* JBIC_ACTION_NOT_FOUND  24 */ "action not found",
};

static constexpr auto MAX_ERROR_CODE = ((sizeof(error_text)/sizeof(error_text[0]))+1);

void JamSTAPL::ReadIdCode() {
	Execute("READ_IDCODE");
}

void JamSTAPL::ReadUsercode() {
	Execute("READ_USERCODE");
}

void JamSTAPL::Erase() {
	Execute("ERASE");;
}

void JamSTAPL::Verify() {
	Execute("VERIFY");;
}

void JamSTAPL::Program() {
	Execute("PROGRAM");
}

const char *JamSTAPL::GetResultString() const {
	if (static_cast<unsigned int>(m_nExecResult) < MAX_ERROR_CODE) {
		return error_text[m_nExecResult];
	}

	return "Unknown error code";
}

const char* JamSTAPL::GetExitCodeString() const {
	switch (m_nExitCode) {
	case 0:
		return "Success";
		break;
	case 1:
		return "Checking chain failure";
		break;
	case 2:
		return "Reading IDCODE failure";
		break;
	case 3:
		return "Reading USERCODE failure";
		break;
	case 4:
		return "Reading UESCODE failure";
		break;
	case 5:
		return "Entering ISP failure";
		break;
	case 6:
		return "Unrecognized device";
		break;
	case 7:
		return "Device revision is not supported";
		break;
	case 8:
		return "Erase failure";
		break;
	case 9:
		return "Device is not blank";
		break;
	case 10:
		return "Device programming failure";
		break;
	case 11:
		return "Device verify failure";
		break;
	case 12:
		return "Read failure";
		break;
	case 13:
		return "Calculating checksum failure";
		break;
	case 14:
		return "Setting security bit failure";
		break;
	case 15:
		return "Querying security bit failure";
		break;
	case 16:
		return "Exiting ISP failure";
		break;
	case 17:
		return "Performing system test failure";
		break;
	default:
		return "Unknown exit code";
		break;
	}
}

void JamSTAPL::PrintInfo() {
	int nActionCount;
	int nProcedureCount;

	jbi_get_file_info(m_pProgram, m_nProgramSize, &m_FormatVersion, &nActionCount, &nProcedureCount);

	printf("File format is %s ByteCode format\n", (m_FormatVersion == 2) ? "Jam STAPL" : "pre-standardized Jam 1.1");
}

JBI_RETURN_TYPE JamSTAPL::CheckCRC(bool bVerbose) {
	unsigned short nExpected;
	unsigned short nActual;

	auto crc_result = jbi_check_crc(m_pProgram, m_nProgramSize, &nExpected, &nActual);

	if (bVerbose || (crc_result == JBIC_CRC_ERROR)) {
		switch (crc_result) {
		case JBIC_SUCCESS:
			printf("CRC matched: CRC value = %04X\n", nActual);
			break;
		case JBIC_CRC_ERROR:
			printf("CRC mismatch: expected %04X, actual %04X\n", nExpected, nActual);
			break;
		case JBIC_UNEXPECTED_END:
			printf("Expected CRC not found, actual CRC value = %04X\n", nActual);
			break;
		case JBIC_IO_ERROR:
			printf("Error: File format is not recognized.\n");
			break;
		default:
			printf("CRC function returned error code %d\n", crc_result);
			break;
		}
	}

	return crc_result;
}

void JamSTAPL::Execute(const char *pAction) {
	DEBUG_PUTS(pAction);

	m_FormatVersion = 2;
	m_nResetJtag = 1;

	m_nExecResult = jbi_execute(m_pProgram, m_nProgramSize,
			nullptr, 0,
			const_cast<char *>(pAction),
			nullptr, m_nResetJtag,
			&m_nErrorAddress,
			&m_nExitCode,
			&m_FormatVersion);
}
