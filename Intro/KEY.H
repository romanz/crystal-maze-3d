#ifndef __KEY_H
#define __KEY_H

enum ScanCode
{
	kSYSREQ              = 0x54,
	kCAPSLOCK            = 0x3A,
	kNUMLOCK             = 0x45,
	kSCROLLLOCK          = 0x46,
	kLEFTCTRL            = 0x1D,
	kLEFTALT             = 0x38,
	kLEFTSHIFT           = 0x2A,
	kRIGHTCTRL           = 0x9D,
	kRIGHTALT            = 0xB8,
	kRIGHTSHIFT          = 0x36,
	kESC                 = 0x01,
	kBACKSPACE           = 0x0E,
	kENTER               = 0x1C,
	kSPACE               = 0x39,
	kTAB                 = 0x0F,
	kF1                  = 0x3B,
	kF2                  = 0x3C,
	kF3                  = 0x3D,
	kF4                  = 0x3E,
	kF5                  = 0x3F,
	kF6                  = 0x40,
	kF7                  = 0x41,
	kF8                  = 0x42,
	kF9                  = 0x43,
	kF10                 = 0x44,
	kF11                 = 0x57,
	kF12                 = 0x58,
	kA                   = 0x1E,
	kB                   = 0x30,
	kC                   = 0x2E,
	kD                   = 0x20,
	kE                   = 0x12,
	kF                   = 0x21,
	kG                   = 0x22,
	kH                   = 0x23,
	kI                   = 0x17,
	kJ                   = 0x24,
	kK                   = 0x25,
	kL                   = 0x26,
	kM                   = 0x32,
	kN                   = 0x31,
	kO                   = 0x18,
	kP                   = 0x19,
	kQ                   = 0x10,
	kR                   = 0x13,
	kS                   = 0x1F,
	kT                   = 0x14,
	kU                   = 0x16,
	kV                   = 0x2F,
	kW                   = 0x11,
	kX                   = 0x2D,
	kY                   = 0x15,
	kZ                   = 0x2C,
	k1                   = 0x02,
	k2                   = 0x03,
	k3                   = 0x04,
	k4                   = 0x05,
	k5                   = 0x06,
	k6                   = 0x07,
	k7                   = 0x08,
	k8                   = 0x09,
	k9                   = 0x0A,
	k0                   = 0x0B,
	kMINUS               = 0x0C,
	kEQUAL               = 0x0D,
	kLBRACKET            = 0x1A,
	kRBRACKET            = 0x1B,
	kSEMICOLON           = 0x27,
	kTICK                = 0x28,
	kAPOSTROPHE          = 0x29,
	kBACKSLASH           = 0x2B,
	kCOMMA               = 0x33,
	kPERIOD              = 0x34,
	kSLASH               = 0x35,
	kINS                 = 0xD2,
	kDEL                 = 0xD3,
	kHOME                = 0xC7,
	kEND                 = 0xCF,
	kPGUP                = 0xC9,
	kPGDN                = 0xD1,
	kLARROW              = 0xCB,
	kRARROW              = 0xCD,
	kUARROW              = 0xC8,
	kDARROW              = 0xD0,
	kKEYPAD0             = 0x52,
	kKEYPAD1             = 0x4F,
	kKEYPAD2             = 0x50,
	kKEYPAD3             = 0x51,
	kKEYPAD4             = 0x4B,
	kKEYPAD5             = 0x4C,
	kKEYPAD6             = 0x4D,
	kKEYPAD7             = 0x47,
	kKEYPAD8             = 0x48,
	kKEYPAD9             = 0x49,
	kKEYPADDEL           = 0x53,
	kKEYPADSTAR          = 0x37,
	kKEYPADMINUS         = 0x4A,
	kKEYPADPLUS          = 0x4E,
	kKEYPADENTER         = 0x9C,
	kCTRLPRTSC           = 0xB7,
	kSHIFTPRTSC          = 0xB7,
	kKEYPADSLASH         = 0xB5
};

class TKeyboard
{

public:

	TKeyboard();
	~TKeyboard();

	int Key(ScanCode code);

	void Clear();

};

extern TKeyboard Keyboard;

#endif