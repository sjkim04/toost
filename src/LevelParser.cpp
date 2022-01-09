﻿#include "LevelParser.hpp"

LevelParser::LevelParser() {
	TileLoc.resize(152);
	for(int i = 0; i < 152; i++) {
		TileLoc[i].resize(4);
	}

	PipeLoc.resize(5);
	for(int i = 0; i < 5; i++) {
		PipeLoc[i].resize(9);
	}

	ObjLocData.resize(2);
	ObjLocData[0].resize(301);
	ObjLocData[1].resize(301);
	for(int i = 0; i < 301; i++) {
		ObjLocData[0][i].resize(301);
		ObjLocData[1][i].resize(301);
	}

	GroundNode.resize(301);
	for(int i = 0; i < 301; i++) {
		GroundNode[i].resize(301);
	}

	TrackYPt.resize(17);
	for(int i = 0; i < 17; i++) {
		TrackYPt[i].resize(4);
	}
}

/*
std::string LevelParser::GetItemName(int n, int v) {
	std::string tempGetItemName = "";
	switch(v) {
	case 12621:
		tempGetItemName = ObjEng[n];
		break;
	case 13133:
		tempGetItemName = ObjEng3[n];
		break;
	case 22349:
		tempGetItemName = ObjEngW[n];
		break;
	case 21847:
		tempGetItemName = ObjEngU[n];
		break;
	case 22323:
		tempGetItemName = ObjEngD[n];
		break;
	default:
		tempGetItemName = "?";
		break;
	}
	return tempGetItemName;
}
*/

bool LevelParser::DecryptLevelData(const std::string& input, const std::string& output) {
	return LevelDecryptor::decrypt(input.c_str(), output.c_str());
}

void LevelParser::LoadLevelData(const std::string& P, bool overworld) {
	isOverworld = overworld;
	int Offset;
	Offset      = overworld ? 0x200 : 0x2E0E0;
	long long i = 0;
	long long j = 0;

	FILE* levelPtr = fopen(P.c_str(), "rb");
	fread(&LH.StartY, sizeof(LH.StartY), 1, levelPtr);
	fread(&LH.GoalY, sizeof(LH.GoalY), 1, levelPtr);
	fread(&LH.GoalX, sizeof(LH.GoalX), 1, levelPtr);
	fread(&LH.Timer, sizeof(LH.Timer), 1, levelPtr);
	fread(&LH.ClearCA, sizeof(LH.ClearCA), 1, levelPtr);
	fread(&LH.DateYY, sizeof(LH.DateYY), 1, levelPtr);
	fread(&LH.DateMM, sizeof(LH.DateMM), 1, levelPtr);
	fread(&LH.DateDD, sizeof(LH.DateDD), 1, levelPtr);
	fread(&LH.DateH, sizeof(LH.DateH), 1, levelPtr);
	fread(&LH.DateM, sizeof(LH.DateM), 1, levelPtr);
	fread(&LH.AutoscrollSpd, sizeof(LH.AutoscrollSpd), 1, levelPtr);
	fread(&LH.ClearCC, sizeof(LH.ClearCC), 1, levelPtr);
	fread(&LH.ClearCRC, sizeof(LH.ClearCRC), 1, levelPtr);
	fread(&LH.GameVer, sizeof(LH.GameVer), 1, levelPtr);
	fread(&LH.MFlag, sizeof(LH.MFlag), 1, levelPtr);
	fread(&LH.ClearAttempts, sizeof(LH.ClearAttempts), 1, levelPtr);
	fread(&LH.ClearTime, sizeof(LH.ClearTime), 1, levelPtr);
	fread(&LH.CreationID, sizeof(LH.CreationID), 1, levelPtr);
	fread(&LH.UploadID, sizeof(LH.UploadID), 1, levelPtr);
	fread(&LH.ClearVer, sizeof(LH.ClearVer), 1, levelPtr);
	fseek(levelPtr, 0xF1, SEEK_SET);
	fread(&LH.GameStyle, sizeof(LH.GameStyle), 1, levelPtr);

	puts("Parsed header");

	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> wstringConverter;

	std::wstring S = L"";
	short K        = 0;
	for(i = 1; i <= 0x42; i += 2) {
		fseek(levelPtr, 0xF3 + i, SEEK_SET);
		fread(&K, sizeof(K), 1, levelPtr);
		if(K == 0) {
			break;
		}
		S += (wchar_t)K;
	}
	LH.Name = wstringConverter.to_bytes(S);

	S = L"";
	for(i = 1; i <= 0xCA; i += 2) {
		fseek(levelPtr, 0x135 + i, SEEK_SET);
		fread(&K, sizeof(K), 1, levelPtr);
		if(K == 0) {
			break;
		}
		S += (wchar_t)K;
	}
	LH.Desc = wstringConverter.to_bytes(S);

	fmt::print("Parsed name ({}) and description\n", LH.Name);

	int32_t M = 0;

	fseek(levelPtr, Offset, SEEK_SET);
	fread(&MapHdr.Theme, sizeof(MapHdr.Theme), 1, levelPtr);
	fread(&MapHdr.AutoscrollType, sizeof(MapHdr.AutoscrollType), 1, levelPtr);
	fread(&MapHdr.BorFlag, sizeof(MapHdr.BorFlag), 1, levelPtr);
	fread(&MapHdr.Ori, sizeof(MapHdr.Ori), 1, levelPtr);
	fread(&MapHdr.LiqEHeight, sizeof(MapHdr.LiqEHeight), 1, levelPtr);
	fread(&MapHdr.LiqMode, sizeof(MapHdr.LiqMode), 1, levelPtr);
	fread(&MapHdr.LiqSpd, sizeof(MapHdr.LiqSpd), 1, levelPtr);
	fread(&MapHdr.LiqSHeight, sizeof(MapHdr.LiqSHeight), 1, levelPtr);
	fread(&MapHdr.BorR, sizeof(MapHdr.BorR), 1, levelPtr);
	fread(&MapHdr.BorT, sizeof(MapHdr.BorT), 1, levelPtr);
	fread(&MapHdr.BorL, sizeof(MapHdr.BorL), 1, levelPtr);
	fread(&MapHdr.BorB, sizeof(MapHdr.BorB), 1, levelPtr);
	fread(&MapHdr.Flag, sizeof(MapHdr.Flag), 1, levelPtr);
	fread(&MapHdr.ObjCount, sizeof(MapHdr.ObjCount), 1, levelPtr);
	fread(&MapHdr.SndCount, sizeof(MapHdr.SndCount), 1, levelPtr);
	fread(&MapHdr.SnakeCount, sizeof(MapHdr.SnakeCount), 1, levelPtr);
	fread(&MapHdr.ClearPipCount, sizeof(MapHdr.ClearPipCount), 1, levelPtr);
	fread(&MapHdr.CreeperCount, sizeof(MapHdr.CreeperCount), 1, levelPtr);
	fread(&MapHdr.iBlkCount, sizeof(MapHdr.iBlkCount), 1, levelPtr);
	fread(&MapHdr.TrackBlkCount, sizeof(MapHdr.TrackBlkCount), 1, levelPtr);
	fseek(levelPtr, Offset + 0x3C, SEEK_SET);
	fread(&MapHdr.GroundCount, sizeof(MapHdr.GroundCount), 1, levelPtr);
	fread(&MapHdr.TrackCount, sizeof(MapHdr.TrackCount), 1, levelPtr);
	fread(&MapHdr.IceCount, sizeof(MapHdr.IceCount), 1, levelPtr);

	puts("Parsed map header");

	MapObj.resize(MapHdr.ObjCount);
	for(M = 0; M < MapHdr.ObjCount; M++) {
		fseek(levelPtr, Offset + 0x48 + M * 0x20, SEEK_SET);
		fread(&MapObj[M].X, sizeof(MapObj[M].X), 1, levelPtr);
		fread(&MapObj[M].Y, sizeof(MapObj[M].Y), 1, levelPtr);
		fseek(levelPtr, Offset + 0x48 + 0xA + M * 0x20, SEEK_SET);
		fread(&MapObj[M].W, sizeof(MapObj[M].W), 1, levelPtr);
		fread(&MapObj[M].H, sizeof(MapObj[M].H), 1, levelPtr);
		fread(&MapObj[M].Flag, sizeof(MapObj[M].Flag), 1, levelPtr);
		fread(&MapObj[M].CFlag, sizeof(MapObj[M].CFlag), 1, levelPtr);
		fread(&MapObj[M].Ex, sizeof(MapObj[M].Ex), 1, levelPtr);
		fread(&MapObj[M].ID, sizeof(MapObj[M].ID), 1, levelPtr);
		fread(&MapObj[M].CID, sizeof(MapObj[M].CID), 1, levelPtr);
		fread(&MapObj[M].LID, sizeof(MapObj[M].LID), 1, levelPtr);
		fread(&MapObj[M].SID, sizeof(MapObj[M].SID), 1, levelPtr);
		MapObj[M].LinkType = 0;
	}

	puts("Parsed map objects");

	// 0x14584  0x4B0 (0x4 * 300)Sound Effect
	//蛇砖块0x149F8  0x12D4 (0x3C4 * 5)Snake Block
	MapSnk.resize(MapHdr.SnakeCount);
	for(M = 0; M < MapHdr.SnakeCount; M++) {
		fseek(levelPtr, Offset + 0x149F8 + M * 0x3C4, SEEK_SET);
		fread(&MapSnk[M].index, sizeof(MapSnk[M].index), 1, levelPtr);
		fread(&MapSnk[M].NodeCount, sizeof(MapSnk[M].NodeCount), 1, levelPtr);

		MapSnk[M].Node.resize(MapSnk[M].NodeCount);

		for(i = 0; i < MapSnk[M].NodeCount; i++) {
			fseek(levelPtr, Offset + 0x149F8 + 0x0 + M * 0x3C4 + i * 0x8, SEEK_SET);
			fread(&MapSnk[M].Node[i].index, sizeof(MapSnk[M].Node[i].index), 1, levelPtr);
			fseek(levelPtr, Offset + 0x149F8 + 0x6 + M * 0x3C4 + i * 0x8, SEEK_SET);
			fread(&MapSnk[M].Node[i].Dir, sizeof(MapSnk[M].Node[i].Dir), 1, levelPtr);
		}
	}

	puts("Parsed snake blocks");

	MapCPipe.resize(MapHdr.ClearPipCount);
	for(M = 0; M < MapHdr.ClearPipCount; M++) {
		fseek(levelPtr, Offset + 0x15CCC + M * 0x124, SEEK_SET);
		fread(&MapCPipe[M].Index, sizeof(MapCPipe[M].Index), 1, levelPtr);
		fread(&MapCPipe[M].NodeCount, sizeof(MapCPipe[M].NodeCount), 1, levelPtr);

		MapCPipe[M].Node.resize(MapCPipe[M].NodeCount);

		for(i = 0; i < MapCPipe[M].NodeCount; i++) {
			fseek(levelPtr, Offset + 0x15CCC + 0x4 + M * 0x124 + i * 0x8, SEEK_SET);
			fread(&MapCPipe[M].Node[i].type, sizeof(MapCPipe[M].Node[i].type), 1, levelPtr);
			fread(&MapCPipe[M].Node[i].index, sizeof(MapCPipe[M].Node[i].index), 1, levelPtr);
			fread(&MapCPipe[M].Node[i].X, sizeof(MapCPipe[M].Node[i].X), 1, levelPtr);
			fread(&MapCPipe[M].Node[i].Y, sizeof(MapCPipe[M].Node[i].Y), 1, levelPtr);
			fread(&MapCPipe[M].Node[i].W, sizeof(MapCPipe[M].Node[i].W), 1, levelPtr);
			fread(&MapCPipe[M].Node[i].H, sizeof(MapCPipe[M].Node[i].H), 1, levelPtr);
			fseek(levelPtr, Offset + 0x15CCC + 0xB + M * 0x124 + i * 0x8, SEEK_SET);
			fread(&MapCPipe[M].Node[i].Dir, sizeof(MapCPipe[M].Node[i].Dir), 1, levelPtr);
		}
	}

	puts("Parsed clear pipe");

	MapCrp.resize(MapHdr.CreeperCount);
	for(M = 0; M < MapHdr.CreeperCount; M++) {
		fseek(levelPtr, Offset + 0x240EC + 0x1 + M * 0x54, SEEK_SET);
		fread(&MapCrp[M].index, sizeof(MapCrp[M].index), 1, levelPtr);
		fread(&MapCrp[M].NodeCount, sizeof(MapCrp[M].NodeCount), 1, levelPtr);

		MapCrp[M].Node.resize(MapCrp[M].NodeCount);

		for(i = 0; i < MapCrp[M].NodeCount; i++) {
			fseek(levelPtr, Offset + 0x240EC + 0x4 + 0x1 + M * 0x54 + i * 0x4, SEEK_SET);
			fread(&MapCrp[M].Node[i], sizeof(MapCrp[M].Node[i]), 1, levelPtr);
		}
	}

	// 0x24434  0x1B8 (0x2C * 10)! Block
	MapMoveBlk.resize(MapHdr.iBlkCount);
	for(M = 0; M < MapHdr.iBlkCount; M++) {
		fseek(levelPtr, Offset + 0x24434 + 0x1 + M * 0x2C, SEEK_SET);
		fread(&MapMoveBlk[M].index, sizeof(MapMoveBlk[M].index), 1, levelPtr);
		fread(&MapMoveBlk[M].NodeCount, sizeof(MapMoveBlk[M].NodeCount), 1, levelPtr);

		MapMoveBlk[M].Node.resize(MapMoveBlk[M].NodeCount);

		for(i = 0; i < MapMoveBlk[M].NodeCount; i++) {
			fseek(levelPtr, Offset + 0x24434 + 0x4 + 0x0 + M * 0x2C + i * 0x4, SEEK_SET);
			fread(&MapMoveBlk[M].Node[i].p0, sizeof(MapMoveBlk[M].Node[i].p0), 1, levelPtr);
			fread(&MapMoveBlk[M].Node[i].p1, sizeof(MapMoveBlk[M].Node[i].p1), 1, levelPtr);
			fread(&MapMoveBlk[M].Node[i].p2, sizeof(MapMoveBlk[M].Node[i].p2), 1, levelPtr);
		}
	}

	// 0x245EC  0x1B8 (0x2C * 10)Track Block
	MapTrackBlk.resize(MapHdr.TrackBlkCount);
	for(M = 0; M < MapHdr.TrackBlkCount; M++) {
		fseek(levelPtr, Offset + 0x245EC + 0x1 + M * 0x2C, SEEK_SET);
		fread(&MapTrackBlk[M].index, sizeof(MapTrackBlk[M].index), 1, levelPtr);
		fread(&MapTrackBlk[M].NodeCount, sizeof(MapTrackBlk[M].NodeCount), 1, levelPtr);

		MapTrackBlk[M].Node.resize(MapTrackBlk[M].NodeCount);

		for(i = 0; i < MapTrackBlk[M].NodeCount; i++) {
			fseek(levelPtr, Offset + 0x245EC + 0x4 + 0x0 + M * 0x2C + i * 0x4, SEEK_SET);
			fread(&MapTrackBlk[M].Node[i].p0, sizeof(MapTrackBlk[M].Node[i].p0), 1, levelPtr);
			fread(&MapTrackBlk[M].Node[i].p1, sizeof(MapTrackBlk[M].Node[i].p1), 1, levelPtr);
			fread(&MapTrackBlk[M].Node[i].p2, sizeof(MapTrackBlk[M].Node[i].p2), 1, levelPtr);
		}
	}

	puts("Parsed tracks");

	MapGrd.resize(MapHdr.GroundCount);

	for(M = 0; M <= 300; M++) {
		for(j = 0; j <= 300; j++) {
			GroundNode[M][j] = 0;
		}
	}
	for(M = 0; M < MapHdr.GroundCount; M++) {
		fseek(levelPtr, Offset + 0x247A4 + M * 0x4, SEEK_SET);
		fread(&MapGrd[M].X, sizeof(MapGrd[M].X), 1, levelPtr);
		fread(&MapGrd[M].Y, sizeof(MapGrd[M].Y), 1, levelPtr);
		fread(&MapGrd[M].ID, sizeof(MapGrd[M].ID), 1, levelPtr);
		fread(&MapGrd[M].BID, sizeof(MapGrd[M].BID), 1, levelPtr);

		GroundNode[MapGrd[M].X + 1][MapGrd[M].Y + 1] = 1;
	}
	if(overworld) {
		for(j = std::round((LH.GoalX - 5) / 10.0); j <= (LH.GoalX - 5) / 10.0 + 9; j++) {
			for(i = 0; i < LH.GoalY; i++) {
				GroundNode[j + 1][i + 1] = 1;
			}
		}
		for(j = 0; j <= 6; j++) {
			for(i = 0; i < LH.StartY; i++) {
				GroundNode[j + 1][i + 1] = 1;
			}
		}
	}

	//轨道0x28624  0x4650 (0xC * 1500)Track
	MapTrk.resize(MapHdr.TrackCount);

	TrackNode.resize(MapHdr.BorR + 3);
	for(int i = 0; i < MapHdr.BorR + 3; i++) {
		TrackNode[i].resize(MapHdr.BorT + 3);
	}

	unsigned char TX = 0;
	for(M = 0; M < MapHdr.TrackCount; M++) {
		fseek(levelPtr, Offset + 0x28624 + 0x0 + M * 0xC, SEEK_SET);
		fread(&MapTrk[M].UN, sizeof(MapTrk[M].UN), 1, levelPtr);
		fread(&MapTrk[M].Flag, sizeof(MapTrk[M].Flag), 1, levelPtr);
		fread(&TX, sizeof(TX), 1, levelPtr);

		if(TX == 255) {
			MapTrk[M].X = 0;
		} else {
			MapTrk[M].X = (unsigned char)(TX + 1);
		}

		fread(&TX, sizeof(TX), 1, levelPtr);

		if(TX == 255) {
			MapTrk[M].Y = 0;
		} else {
			MapTrk[M].Y = (unsigned char)(TX + 1);
		}

		fread(&MapTrk[M].Type, sizeof(MapTrk[M].Type), 1, levelPtr);
		fread(&MapTrk[M].LID, sizeof(MapTrk[M].LID), 1, levelPtr);
		fread(&MapTrk[M].K0, sizeof(MapTrk[M].K0), 1, levelPtr);
		fread(&MapTrk[M].K1, sizeof(MapTrk[M].K1), 1, levelPtr);

		switch(MapTrk[M].Type) {
		case 0:
			TrackNode[MapTrk[M].X][MapTrk[M].Y + 1] += 1;
			TrackNode[MapTrk[M].X + 2][MapTrk[M].Y + 1] += 1;
			MapTrk[M].F0 = (unsigned char)(((MapTrk[M].K0 + 0x10000) / 0x80) % 2);
			MapTrk[M].F1 = (unsigned char)(((MapTrk[M].K1 + 0x10000) / 0x80) % 2);
			break;
		case 1:
			TrackNode[MapTrk[M].X + 1][MapTrk[M].Y + 2] += 1;
			TrackNode[MapTrk[M].X + 1][MapTrk[M].Y] += 1;
			MapTrk[M].F0 = (unsigned char)(((MapTrk[M].K0 + 0x10000) / 0x80) % 2);
			MapTrk[M].F1 = (unsigned char)(((MapTrk[M].K1 + 0x10000) / 0x80) % 2);
			break;
		case 2:
		case 4:
		case 5:
			TrackNode[MapTrk[M].X][MapTrk[M].Y + 2] += 1;
			TrackNode[MapTrk[M].X + 2][MapTrk[M].Y] += 1;
			MapTrk[M].F0 = (unsigned char)(((MapTrk[M].K0 + 0x10000) / 0x80) % 2);
			MapTrk[M].F1 = (unsigned char)(((MapTrk[M].K1 + 0x10000) / 0x80) % 2);
			break;
		case 3:
		case 6:
		case 7:
			TrackNode[MapTrk[M].X + 2][MapTrk[M].Y + 2] += 1;
			TrackNode[MapTrk[M].X][MapTrk[M].Y] += 1;
			MapTrk[M].F0 = (unsigned char)(((MapTrk[M].K0 + 0x10000) / 0x80) % 2);
			MapTrk[M].F1 = (unsigned char)(((MapTrk[M].K1 + 0x10000) / 0x80) % 2);
			break;
		case 8:
			TrackNode[MapTrk[M].X][MapTrk[M].Y + 2] += 1;
			TrackNode[MapTrk[M].X + 4][MapTrk[M].Y] += 1;
			TrackNode[MapTrk[M].X + 4][MapTrk[M].Y + 4] += 1;
			MapTrk[M].F0 = (unsigned char)(((MapTrk[M].K1 + 0x10000) / 0x80) % 2);
			MapTrk[M].F1 = (unsigned char)(((MapTrk[M].K0 + 0x10000) / 0x80) % 2);
			MapTrk[M].F2 = (unsigned char)(((MapTrk[M].K0 + 0x10000) / 0x8000) % 2);
			break;
		case 9:
			TrackNode[MapTrk[M].X][MapTrk[M].Y] += 1;
			TrackNode[MapTrk[M].X][MapTrk[M].Y + 4] += 1;
			TrackNode[MapTrk[M].X + 4][MapTrk[M].Y + 2] += 1;
			MapTrk[M].F0 = (unsigned char)(((MapTrk[M].K1 + 0x10000) / 0x40) % 2);
			MapTrk[M].F1 = (unsigned char)(((MapTrk[M].K1 + 0x10000) / 0x2) % 2);
			MapTrk[M].F2 = (unsigned char)(((MapTrk[M].K0 + 0x10000) / 0x80) % 2);
			break;
		case 10:
			TrackNode[MapTrk[M].X][MapTrk[M].Y] += 1;
			TrackNode[MapTrk[M].X + 2][MapTrk[M].Y + 4] += 1;
			TrackNode[MapTrk[M].X + 4][MapTrk[M].Y] += 1;
			MapTrk[M].F0 = (unsigned char)(1 - ((MapTrk[M].K0 + 0x10000) / 0x4000) % 2);
			MapTrk[M].F1 = (unsigned char)(((MapTrk[M].K1 + 0x10000) / 0x40) % 2);
			MapTrk[M].F2 = (unsigned char)(((MapTrk[M].K0 + 0x10000) / 0x80) % 2);
			break;
		case 11:
			TrackNode[MapTrk[M].X + 2][MapTrk[M].Y] += 1;
			TrackNode[MapTrk[M].X][MapTrk[M].Y + 4] += 1;
			TrackNode[MapTrk[M].X + 4][MapTrk[M].Y + 4] += 1;
			MapTrk[M].F0 = (unsigned char)(((MapTrk[M].K0 + 0x10000) / 0x80) % 2);
			MapTrk[M].F1 = (unsigned char)(((MapTrk[M].K1 + 0x10000) / 0x2) % 2);
			MapTrk[M].F2 = (unsigned char)(((MapTrk[M].K1 + 0x10000) / 0x40) % 2);
			break;
		case 12:
			TrackNode[MapTrk[M].X][MapTrk[M].Y + 2] += 1;
			TrackNode[MapTrk[M].X + 4][MapTrk[M].Y] += 1;
			TrackNode[MapTrk[M].X + 4][MapTrk[M].Y + 4] += 1;
			MapTrk[M].F0 = (unsigned char)(((MapTrk[M].K1 + 0x10000) / 0x800) % 2);
			MapTrk[M].F1 = (unsigned char)(((MapTrk[M].K0 + 0x10000) / 0x80) % 2);
			MapTrk[M].F2 = (unsigned char)(((MapTrk[M].K0 + 0x10000) / 0x1000) % 2);
			break;
		case 13:
			TrackNode[MapTrk[M].X][MapTrk[M].Y] += 1;
			TrackNode[MapTrk[M].X][MapTrk[M].Y + 4] += 1;
			TrackNode[MapTrk[M].X + 4][MapTrk[M].Y + 2] += 1;
			MapTrk[M].F0 = (unsigned char)(((MapTrk[M].K1 + 0x10000) / 0x800) % 2);
			MapTrk[M].F1 = (unsigned char)(((MapTrk[M].K0 + 0x10000) / 0x1000) % 2);
			MapTrk[M].F2 = (unsigned char)(((MapTrk[M].K0 + 0x10000) / 0x80) % 2);
			break;
		case 14:
			TrackNode[MapTrk[M].X][MapTrk[M].Y] += 1;
			TrackNode[MapTrk[M].X + 4][MapTrk[M].Y] += 1;
			TrackNode[MapTrk[M].X + 2][MapTrk[M].Y + 4] += 1;
			MapTrk[M].F0 = (unsigned char)(((MapTrk[M].K0 + 0x10000) / 0x1000) % 2);
			MapTrk[M].F1 = (unsigned char)(((MapTrk[M].K1 + 0x10000) / 0x800) % 2);
			MapTrk[M].F2 = (unsigned char)(((MapTrk[M].K0 + 0x10000) / 0x80) % 2);
			break;
		case 15:
			TrackNode[MapTrk[M].X + 2][MapTrk[M].Y] += 1;
			TrackNode[MapTrk[M].X][MapTrk[M].Y + 4] += 1;
			TrackNode[MapTrk[M].X + 4][MapTrk[M].Y + 4] += 1;
			MapTrk[M].F0 = (unsigned char)(((MapTrk[M].K0 + 0x10000) / 0x80) % 2);
			MapTrk[M].F1 = (unsigned char)(((MapTrk[M].K0 + 0x10000) / 0x1000) % 2);
			MapTrk[M].F2 = (unsigned char)(((MapTrk[M].K1 + 0x10000) / 0x800) % 2);
			break;
		}
	}
	//冰块0x2CC74  0x4B0 (0x4 * 300)Icicle
	MapIce.resize(MapHdr.IceCount);
	for(M = 0; M < MapHdr.IceCount; M++) {
		fseek(levelPtr, Offset + 0x2CC74 + 0x0 + M * 0x4, SEEK_SET);
		fread(&MapIce[M].X, sizeof(MapIce[M].X), 1, levelPtr);
		fread(&MapIce[M].Y, sizeof(MapIce[M].Y), 1, levelPtr);
		fread(&MapIce[M].ID, sizeof(MapIce[M].ID), 1, levelPtr);
	}

	puts("Done parsing");

	fclose(levelPtr);
}

void LevelParser::ExportToJSON(const std::string& outputPath, std::vector<DrawingInstruction>& drawingInstructions) {
	rapidjson::StringBuffer sb;
	rapidjson::Writer<rapidjson::StringBuffer> writer(sb);

	writer.StartObject();
	writer.Key("name");
	writer.String(LH.Name);
	writer.Key("description");
	writer.String(LH.Desc);
	writer.Key("gamestyle");
	writer.String(levelMappings->NumToGameStyle.at(LH.GameStyle));
	writer.Key("gamestyle_raw");
	writer.Int(LH.GameStyle);
	writer.Key("theme");
	writer.String(levelMappings->NumToTheme.at(MapHdr.Theme));
	writer.Key("theme_raw");
	writer.Int(MapHdr.Theme);
	writer.Key("is_overworld");
	writer.Bool(isOverworld);
	writer.Key("night_time");
	writer.Bool(MapHdr.Flag == 1);
	writer.Key("clear_time");
	writer.Int(LH.ClearTime);
	writer.Key("clear_time_pretty");
	writer.String(levelMappings->FormatMillisecondTime(LH.ClearTime));
	writer.Key("clear_attempts");
	writer.Int(LH.ClearAttempts);
	writer.Key("game_version");
	writer.String(levelMappings->NumToGameVersion.at(LH.ClearVer));
	writer.Key("game_version_raw");
	writer.Int(LH.ClearVer);
	writer.Key("timer");
	writer.Int(LH.Timer);
	writer.Key("start_y");
	writer.Int(LH.StartY);
	writer.Key("goal_x");
	writer.Int(LH.GoalX);
	writer.Key("goal_y");
	writer.Int(LH.GoalY);
	writer.Key("clear_condition_type");
	writer.String(levelMappings->NumToClearCondition.at(LH.ClearCRC));
	writer.Key("clear_condition_type_raw");
	writer.Int(LH.ClearCRC);
	writer.Key("clear_condition_magnitude");
	writer.Int(LH.ClearCA);
	writer.Key("clear_condition");
	writer.String(fmt::format(levelMappings->NumToClearCondition.at(LH.ClearCRC), LH.ClearCA));
	writer.Key("clear_condition_category");
	writer.String(levelMappings->NumToClearConditionCategory.at(LH.ClearCC));
	writer.Key("clear_condition_category_raw");
	writer.Int(LH.ClearCC);
	writer.Key("autoscroll_speed");
	writer.String(levelMappings->NumToAutoscrollSpeed.at(LH.AutoscrollSpd));
	writer.Key("autoscroll_speed_raw");
	writer.Int(LH.AutoscrollSpd);
	writer.Key("autoscroll_type");
	writer.String(levelMappings->NumToAutoscrollType.at(MapHdr.AutoscrollType));
	writer.Key("autoscroll_type_raw");
	writer.Int(MapHdr.AutoscrollType);
	writer.Key("orientation");
	writer.String(levelMappings->NumToOrientation.at(MapHdr.Ori));
	writer.Key("orientation_raw");
	writer.Int(MapHdr.Ori);
	writer.Key("liquid_start_height");
	writer.Int(MapHdr.LiqSHeight);
	writer.Key("liquid_end_height");
	writer.Int(MapHdr.LiqEHeight);
	writer.Key("liquid_mode");
	writer.String(levelMappings->NumToLiquidMode.at(MapHdr.LiqMode));
	writer.Key("liquid_speed");
	writer.String(levelMappings->NumToLiquidSpeed.at(MapHdr.LiqSpd));
	writer.Key("boundary_type");
	writer.String(levelMappings->NumToBoundaryType.at(MapHdr.BorFlag));
	writer.Key("liquid_mode_raw");
	writer.Int(MapHdr.LiqMode);
	writer.Key("liquid_speed_raw");
	writer.Int(MapHdr.LiqSpd);
	writer.Key("boundary_type_raw");
	writer.Int(MapHdr.BorFlag);
	writer.Key("right_boundary");
	writer.Int(MapHdr.BorR);
	writer.Key("top_boundary");
	writer.Int(MapHdr.BorT);
	writer.Key("left_boundary");
	writer.Int(MapHdr.BorL);
	writer.Key("bottom_boundary");
	writer.Int(MapHdr.BorB);
	writer.Key("object_count");
	writer.Int(MapHdr.ObjCount);
	writer.Key("sound_effect_count");
	writer.Int(MapHdr.SndCount);
	writer.Key("snake_block_count");
	writer.Int(MapHdr.SnakeCount);
	writer.Key("clear_pipe_count");
	writer.Int(MapHdr.ClearPipCount);
	writer.Key("piranha_creeper_count");
	writer.Int(MapHdr.CreeperCount);
	writer.Key("exclamation_mark_block_count");
	writer.Int(MapHdr.iBlkCount);
	writer.Key("track_count");
	writer.Int(MapHdr.TrackCount);
	writer.Key("ground_count");
	writer.Int(MapHdr.GroundCount);
	writer.Key("icicle_count");
	writer.Int(MapHdr.IceCount);
	writer.Key("upload_id");
	writer.Int64(LH.UploadID);
	writer.Key("creation_id");
	writer.Uint(LH.CreationID);
	writer.Key("gamever");
	writer.Int(LH.GameVer);
	writer.Key("management_flags");
	writer.Int(LH.MFlag);

	writer.Key("objects");
	writer.StartArray();
	for(auto& obj : MapObj) {
		writer.StartObject();
		writer.Key("name");
		writer.String(ObjEng[obj.ID]);
		writer.Key("x");
		writer.Int(obj.X);
		writer.Key("y");
		writer.Int(obj.Y);
		writer.Key("w");
		writer.Int(obj.W);
		writer.Key("h");
		writer.Int(obj.H);
		writer.Key("flag");
		writer.Int(obj.Flag);
		writer.Key("cflag");
		writer.Int(obj.CFlag);
		writer.Key("ex");
		writer.Int(obj.Ex);
		writer.Key("id");
		writer.Int(obj.ID);
		writer.Key("cid");
		writer.Int(obj.CID);
		writer.Key("lid");
		writer.Int(obj.LID);
		writer.Key("sid");
		writer.Int(obj.SID);
		writer.Key("link_type");
		writer.Int(obj.LinkType);
		writer.EndObject();
	}
	writer.EndArray();

	writer.Key("ground");
	writer.StartArray();
	for(auto& ground : MapGrd) {
		writer.StartObject();
		writer.Key("x");
		writer.Int(ground.X);
		writer.Key("y");
		writer.Int(ground.Y);
		writer.Key("id");
		writer.Int(ground.ID);
		writer.Key("bid");
		writer.Int(ground.BID);
		writer.EndObject();
	}
	writer.EndArray();

	writer.Key("track");
	writer.StartArray();
	for(auto& track : MapTrk) {
		writer.StartObject();
		writer.Key("x");
		writer.Int(track.X);
		writer.Key("y");
		writer.Int(track.Y);
		writer.Key("un");
		writer.Int(track.UN);
		writer.Key("flag");
		writer.Int(track.Flag);
		writer.Key("type");
		writer.Int(track.Type);
		writer.Key("lid");
		writer.Int(track.LID);
		writer.Key("k0");
		writer.Int(track.K0);
		writer.Key("k1");
		writer.Int(track.K1);
		writer.Key("f0");
		writer.Int(track.F0);
		writer.Key("f1");
		writer.Int(track.F1);
		writer.Key("f2");
		writer.Int(track.F2);
		writer.EndObject();
	}
	writer.EndArray();

	writer.Key("clear_pipes");
	writer.StartArray();
	for(auto& pipe : MapCPipe) {
		writer.StartObject();
		writer.Key("index");
		writer.Int(pipe.Index);
		writer.Key("node_count");
		writer.Int(pipe.NodeCount);
		writer.Key("nodes");
		writer.StartArray();
		for(auto& node : pipe.Node) {
			writer.StartObject();
			writer.Key("x");
			writer.Int(node.X);
			writer.Key("y");
			writer.Int(node.Y);
			writer.Key("type");
			writer.Int(node.type);
			writer.Key("index");
			writer.Int(node.index);
			writer.Key("w");
			writer.Int(node.W);
			writer.Key("h");
			writer.Int(node.H);
			writer.Key("direction");
			writer.Int(node.Dir);
			writer.EndObject();
		}
		writer.EndArray();
		writer.EndObject();
	}
	writer.EndArray();

	writer.Key("snakes");
	writer.StartArray();
	for(auto& snake : MapSnk) {
		writer.StartObject();
		writer.Key("index");
		writer.Int(snake.index);
		writer.Key("node_count");
		writer.Int(snake.NodeCount);
		writer.Key("nodes");
		writer.StartArray();
		for(auto& node : snake.Node) {
			writer.StartObject();
			writer.Key("index");
			writer.Int(node.index);
			writer.Key("direction");
			writer.Int(node.Dir);
			writer.EndObject();
		}
		writer.EndArray();
		writer.EndObject();
	}
	writer.EndArray();

	writer.Key("piranha_creepers");
	writer.StartArray();
	for(auto& creeper : MapCrp) {
		writer.StartObject();
		writer.Key("index");
		writer.Int(creeper.index);
		writer.Key("node_count");
		writer.Int(creeper.NodeCount);
		writer.Key("nodes");
		writer.StartArray();
		for(auto& node : creeper.Node) {
			writer.Int(node);
		}
		writer.EndArray();
		writer.EndObject();
	}
	writer.EndArray();

	writer.Key("drawing_instructions");
	writer.StartArray();
	for(auto& instruction : drawingInstructions) {
		writer.StartObject();
		writer.Key("path");
		writer.String(instruction.path);
		writer.Key("is_tile");
		writer.Bool(instruction.isTile);
		writer.Key("x");
		writer.Int(instruction.x);
		writer.Key("y");
		writer.Int(instruction.y);
		writer.Key("target_width");
		writer.Int(instruction.targetWidth);
		writer.Key("target_height");
		writer.Int(instruction.targetHeight);
		if(instruction.isTile) {
			writer.Key("tile_x");
			writer.Int(instruction.tileX);
			writer.Key("tile_y");
			writer.Int(instruction.tileY);
			writer.Key("tile_w");
			writer.Int(instruction.tileW);
			writer.Key("tile_h");
			writer.Int(instruction.tileH);
		} else {
			writer.Key("angle");
			writer.Double(instruction.angle);
			writer.Key("opacity");
			writer.Double(instruction.opacity);
		}
		writer.EndObject();
	}
	writer.EndArray();

	writer.EndObject();

	if(std::filesystem::exists(outputPath)) {
		std::filesystem::remove(outputPath);
	}
	std::string output    = sb.GetString();
	auto destination_file = std::fstream(outputPath, std::ios::out);
	destination_file.write(output.c_str(), output.size());
	destination_file.close();
}

const char* ObjEng[] = { "굼바", "엉금엉금", "뻐끔플라워", "해머브러스", "블록", "?블록", "돌", "딱딱블록",
	"코인", "토관", "점프대", "리프트", "쿵쿵", "킬러대포", "버섯 지형", "폭탄병",
	"반벽 지형", "다리", "P스위치", "POW블록", "슈퍼버섯", "도넛블록", "구름블록", "음표블록",
	"파이어바", "가시돌이", "딱딱블록", "골", "하잉바", "투명블록", "김수한무", "김수한무 구름", "매그넘킬러",
	"1UP버섯", "파이어플라워", "슈퍼스타", "용암리프트", "시작 벽돌", "시작 화살표", "마귀", "가시하잉바",
	"부끄부끄", "쿠파피에로", "가봉", "점보버섯", "신발굼바", "와르르", "발사대", "Blooper", "Castle Bridge",
	"Jumping Machine", "폴찍폴찍", "Wiggler", "벨트컨베이어", "버너", "문", "Cheep Cheep", "블랙뻐끔",
	"뚜", "Track", "Lava Bubble", "Chain Chomp", "쿠파", "Ice Block", "덩굴", "Stingby", "Arrow", "일방통행",
	"Saw", "플레이어", "10+코인", "Half Collision Platform", "Koopa Car", "키노피오", "Spike/Ball",
	"돌", "Twister", "붕붕", "Pokey", "P블록", "대시블록", "마리오 2의 버섯", "Donut", "Skewer",
	"스네이크블록", "가시블록", "Charvaargh", "완만한 비탈길", "가파른 비탈길", "Reel Camera", "Checkpoint Flag",
	"시소", "핑크코인", "투명토관", "벨트컨베이어", "열쇠", "Ant Trooper", "워프박스", "쿠파주니어", "ON/OFF스위치",
	"점선블록", "Water Marker", "Monty Mole", "Fish Bone", "Angry Sun", "그네크레인", "나무",
	"쭉쭉뻐끔", "깜빡블록", "사운드 이펙트", "가시블록", "메카쿠파", "Crate", "버섯트램펄린",
	"Porkupuffer", "Cinobic", "Super Hammer", "Bully", "고드름", "!블록", "레미", "모톤", "래리", "웬디", "이기",
	"로이", "루드윅", "Cannon Box", "Propeller Box", "Goomba Mask", "Bullet Bill Mask", "Red POW Box",
	"스위치버섯트램펄린" };

LevelParserMappings* levelMappings = new LevelParserMappings();
