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

#include "TileUtility.h"


int TileUtility::RotateCW(int tile)
{
	switch (tile)
	{
	case kTileCornerFlatTL:
	case kTileCornerFlatTR:
	case kTileCornerFlatBR:
	case kTileCornerBankedTL:
	case kTileCornerBankedTR:
	case kTileCornerBankedBR:
	case kTileStartDown:
	case kTileStartLeft:
	case kTileStartUp:
	case kTileEndUp:
	case kTileEndRight:
	case kTileEndDown:
	case kTunnelEntranceV:
	case kTunnelThroughV:
	case kTunnelEntranceH:
	case kTunnelThroughH:
	case kCornerLargeTL1:
	case kCornerLargeTL2:
	case kCornerLargeTL3:
	case kCornerLargeTR1:
	case kCornerLargeTR2:
	case kCornerLargeTR3:
	case kCornerLargeBR1:
	case kCornerLargeBR2:
	case kCornerLargeBR3:
	case kCornerLargeBL1:
	case kCornerLargeBL2:
	case kCornerLargeBL3:
	case kSBendHTB1:
	case kSBendHTB2:
	case kSBendHTB3:
	case kSBendHBT1:
	case kSBendHBT2:
	case kSBendHBT3:
	case kSBendVRL1:
	case kSBendVRL2:
	case kSBendVRL3:
	case kSBendVLR1:
	case kSBendVLR2:
	case kSBendVLR3:
		return ++tile;

	case kTileCornerFlatBL:
		return kTileCornerFlatTL;

	case kTileCornerBankedBL:
		return kTileCornerBankedTL;

	case kTileStraightH:
		return kTileStraightV;

	case kTileStraightV:
		return kTileStraightH;

	case kTileStraightHJump:
		return kTileStraightVJump;

	case kTileStraightVJump:
		return kTileStraightHJump;

	case kTileStraightHWaterJump:
		return kTileStraightVWaterJump;

	case kTileStraightVWaterJump:
		return kTileStraightHWaterJump;

	case kTileStartRight:
		return kTileStartDown;

	case kTileEndLeft:
	  return kTileEndUp;

	case kBridgeH:
		return kBridgeV;

	case kBridgeV:
		return kBridgeH;

	case kTunnelExitV:
		return kTunnelEntranceV;

	case kTunnelExitH:
		return kTunnelEntranceH;

	case kCornerLargeTL4:
		return kCornerLargeTL1;

	case kCornerLargeTR4:
		return kCornerLargeTR1;

	case kCornerLargeBR4:
		return kCornerLargeBR4;

	case kCornerLargeBL4:
		return kCornerLargeBL1;

	case kSBendHTB4:
		return kSBendHTB1;

	case kSBendHBT4:
		return kSBendHBT1;

	case kSBendVRL4:
		return kSBendVRL1;

	case kSBendVLR4:
		return kSBendVLR1;

	case kChicaneH:
		return kChicaneV;

	case kChicaneV:
		return kChicaneH;

	default:
		return tile;
	}
}


// top left tile 1 2
//               3 4
int TileUtility::RotateQuadCW(int tile_tl)
{
	switch (tile_tl)
	{
	case kCornerLargeTL1:
		return kCornerLargeTR1;

	case kCornerLargeTR1:
		return kCornerLargeBR1;

	case kCornerLargeBR1:
		return kCornerLargeBL1;

	case kCornerLargeBL1:
		return kCornerLargeTL1;

	case kSBendHTB1:
		return kSBendHBT1;

	case kSBendHBT1:
		return kSBendVRL1;

	case kSBendVRL1:
		return kSBendVLR1;

	case kSBendVLR1:
		return kSBendHTB1;
	}

    return tile_tl;
}
