// =======================================================================
//
// Extreme Snail Manager
//
// (c) Paul Alan Freshney 1995-2024
// paul@freshney.org
//
// https://github.com/MaximumOctopus/ExtremeSnailManager
//
// =======================================================================

#include "ConstantsTrack.h"


TrackDirection TrackHelper::NextClockwise(TrackDirection td)
{
	switch (td)
	{
		case TrackDirection::kNorth:
			return  TrackDirection::kEast;
		case TrackDirection::kEast:
			return  TrackDirection::kSouth;
		case TrackDirection::kSouth:
			return  TrackDirection::kWest;
		case TrackDirection::kWest:
			return  TrackDirection::kNorth;
	}
}


TrackDirection TrackHelper::Flip180(TrackDirection td)
{
	switch (td)
	{
		case TrackDirection::kNorth:
			return  TrackDirection::kSouth;
		case TrackDirection::kEast:
			return  TrackDirection::kWest;
		case TrackDirection::kSouth:
			return  TrackDirection::kNorth;
		case TrackDirection::kWest:
			return  TrackDirection::kEast;
	}
}


std::wstring TrackHelper::TrackDirectionToStr(TrackDirection td)
{
	switch (td)
	{
		case TrackDirection::kNorth:
			return L"North";
		case TrackDirection::kEast:
			return L"East";
		case TrackDirection::kSouth:
			return L"South";
		case TrackDirection::kWest:
			return L"West";
	}
}


bool TrackHelper::IsCorner(int tile)
{
	switch (tile)
	{
	case kTileCornerFlatTL:
	case kTileCornerFlatTR:
	case kTileCornerFlatBR:
	case kTileCornerFlatBL:
	case kTileCornerBankedTL:
	case kTileCornerBankedTR:
	case kTileCornerBankedBR:
	case kTileCornerBankedBL:
	case kCornerLargeTL1:
	case kCornerLargeTL2:
	case kCornerLargeTL3:
	case kCornerLargeTL4:
	case kCornerLargeTR1:
	case kCornerLargeTR2:
	case kCornerLargeTR3:
	case kCornerLargeTR4:
	case kCornerLargeBR1:
	case kCornerLargeBR2:
	case kCornerLargeBR3:
	case kCornerLargeBR4:
	case kCornerLargeBL1:
	case kCornerLargeBL2:
	case kCornerLargeBL3:
	case kCornerLargeBL4:
	case kSBendHTB1:
	case kSBendHTB2:
	case kSBendHTB3:
	case kSBendHTB4:
	case kSBendHBT1:
	case kSBendHBT2:
	case kSBendHBT3:
	case kSBendHBT4:
	case kSBendVRL1:
	case kSBendVRL2:
	case kSBendVRL3:
	case kSBendVRL4:
	case kSBendVLR1:
	case kSBendVLR2:
	case kSBendVLR3:
	case kSBendVLR4:
		return true;
	}

	return false;
}


BasicTrackType TrackHelper::GetBasicTrackType(int tile)
{
	switch (tile)
	{
	case kTileCornerFlatTL:
	case kTileCornerBankedTL:
	case kCornerLargeTL1:
	case kSBendHBT1:
	case kSBendVRL3:
		return BasicTrackType::kCurveTL;

	case kTileCornerFlatTR:
	case kTileCornerBankedTR:
	case kCornerLargeTR2:
	case kSBendHTB2:
    case kSBendVLR4:
		return BasicTrackType::kCurveTR;

	case kTileCornerFlatBR:
	case kTileCornerBankedBR:
	case kCornerLargeBR1:
	case kCornerLargeBR4:
	case kSBendHBT4:
	case kSBendHBT3:
	case kSBendVRL4:
		return BasicTrackType::kCurveBR;

	case kTileCornerFlatBL:
	case kTileCornerBankedBL:
	case kCornerLargeBL3:
	case kSBendHTB4:
    case kSBendVLR3:
		return BasicTrackType::kCurveBL;

	case kTileStraightH:
	case kTileStraightHJump:
	case kTileStraightHWaterJump:
	case kTunnelEntranceH:
	case kTunnelThroughH:
	case kTunnelExitH:
	case kCornerLargeTL2:
	case kCornerLargeTR1:
	case kCornerLargeBR3:
	case kCornerLargeBL4:
	case kSBendHTB1:
	case kSBendHBT2:
		return BasicTrackType::kHorizontal;

	case kTileStraightV:
	case kTileStraightVJump:
	case kTileStraightVWaterJump:
	case kTunnelEntranceV:
	case kTunnelThroughV:
	case kTunnelExitV:
	case kCornerLargeTL3:
	case kCornerLargeTR4:
	case kCornerLargeBR2:
	case kCornerLargeBL1:
	case kSBendVRL2:
	case kSBendVLR1:
		return BasicTrackType::kVertical;

	case kTileCrossroads:
	case kBridgeH:
	case kBridgeV:
		return BasicTrackType::kCrossroads;

	case kTileStartUp:
	case kTileStartRight:
	case kTileStartDown:
	case kTileStartLeft:
		return BasicTrackType::kStart;

	case kTileEndUp:
	case kTileEndRight:
	case kTileEndDown:
	case kTileEndLeft:
		return BasicTrackType::kEnd;

	default:
		return BasicTrackType::kNone;
	}
}
