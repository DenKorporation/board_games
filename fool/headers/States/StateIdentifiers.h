#pragma once

namespace States
{
	enum ID
	{
		None,
		Menu,
		MultiplayerConnection,
		MultiplayerGame,
		CreatingRoom,
		ConnectionWait,
		Game,
		Pause,
		Statistic,
		Settings,
		GameEnd
	};
}