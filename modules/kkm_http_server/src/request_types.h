#pragma once
#include <stdint.h>
#include <string>
#include <vector>


#define OP_TYPE_CHECK                           1
#define OP_TYPE_RETURN_CHECK                    2
#define OP_TYPE_CANCEL_CHECK                    3
#define OP_TYPE_REPORT_Z                        4
#define OP_TYPE_REPORT_X                        5
#define OP_TYPE_GET_DEVICE                      6
#define OP_TYPE_GET_MODE                        7
#define OP_TYPE_RESET_MODE                      8
#define OP_TYPE_ANNULATE_CHECK                  9
#define OP_TYPE_EXEC_COMMAND                    10
#define OP_TYPE_QUERY_KKM_STATE                 11
#define OP_TYPE_PRINT_PLAIN_TEXT                12


#define OP_TYPE_JPOS_PRINT                      101


//----------------------------------------------------------------------------------------
//for header
struct THeader
{
    std::string  sDevice;
	uint32_t     nTypeOp;
};

//----------------------------------------------------------------------------------------
//for check
enum EDiscountType
{
    kDiscountType_Percent = 0,
    kDiscountType_Summ,
};

enum EDiscountSign
{
	kDiscountSign_Minus = 0,
    kDiscountSign_Plus 
};

struct TDiscount
{
    EDiscountType kDiscountType;
    EDiscountSign kSign; 
    uint32_t      nValue;
};

struct TCheckItem
{
    std::string sPrintLine;
    uint32_t    nPrice;
    uint32_t    nQuantity;
	uint8_t     nDepartment;
    bool        bUseDiscount;
    TDiscount   discount;
};


struct TCheck
{
    std::vector <TCheckItem> vCheckItem;
    bool        bUseAllDiscount;
    TDiscount   discount;
	uint8_t     nTypePayment;
	uint32_t    nPassword;
	uint32_t    nCheckSumm;
};
//----------------------------------------------------------------------------------------
//return operation
struct TReturnCheckItem
{
	std::string sPrintLine;
    uint32_t    nPrice;
    uint32_t    nQuantity;
};

struct TReturnCheck
{
    std::vector <TReturnCheckItem> vCheckItem;
	uint8_t     nTypePayment;
	uint32_t    nPassword;
};

//----------------------------------------------------------------------------------------
//cancel operation
struct TCancellationCheckItem
{
	std::string sPrintLine;
    uint32_t    nPrice;
    uint32_t    nQuantity;
};

struct TCancellationCheck
{
    std::vector <TCancellationCheckItem> vCheckItem;
	uint8_t     nTypePayment;
	uint32_t    nPassword;
};

//----------------------------------------------------------------------------------------
//report x operation
struct TReportX
{
	uint8_t     cReportType;
	uint32_t    nPassword;
};

//----------------------------------------------------------------------------------------
//report z operation
struct TReportZ
{
	uint32_t    nPassword;
};

//----------------------------------------------------------------------------------------
//exec custom command ONLY for TEST
struct TExecCommand
{
	std::string    sExecCommand;
};

//----------------------------------------------------------------------------------------
//print plain text 
struct TPrintText
{
    std::vector <std::string> vStringLine;
};







