#pragma once

#include <SFML/Audio/SoundBuffer.hpp>

namespace sf
{
	class Texture;
	class Font;
}

namespace Textures
{
	namespace Global
	{
		// Card::Suit transform into Textures::Global::ID
		enum ID
		{
			None = 0,
			Diamonds = 1,
			Hearts = 2,
			Clubs = 3,
			Spades = 4,
			Background,
			ReverseFace,
			PauseMenu,
			MainMenu,
		};
	}

	namespace Local
	{
		enum ID
		{
			None = 0,
			Main = 1,
			_2 = 2,
			_3,
			_4,
			_5,
			_6,
			_7,
			_8,
			_9,
			_10,
			_Jack,
			_Queen,
			_King,
			_Ace
		};
	}

	class ID
	{
	private:
		Global::ID global;
		Local::ID local;

	public:
		ID() : global(Global::None), local(Local::None) {}
		ID(Global::ID global) : global(global), local(Local::None) {}
		ID(Global::ID global, Local::ID local) : global(global), local(local) {}

		void set(Global::ID global)
		{
			this->global = global;
			this->local = Local::None;
		}

		void set(Global::ID global, Local::ID local)
		{
			this->global = global;
			this->local = local;
		}

		Global::ID getGlobal()
		{
			return global;
		}

		Local::ID getLocal()
		{
			return local;
		}

		bool operator==(ID const &id) const
		{
			return (this->global == id.global) && (this->local == local);
		}

		bool operator<(ID const &id) const
		{
			if (this->global == id.global)
			{
				return this->local < id.local;
			}
			else
			{
				return this->global < id.global;
			}
		}
	};
}

namespace Fonts
{
	enum ID
	{
		Main,
		Label
	};
}

namespace Sounds
{
	enum ID
	{
		CardSelect,
		ButtonSelect,
		Error,
		Shuffle,
		CardFlip
	};
}

namespace Musics
{
	enum ID
	{
		DrawTheme,
		VictoryTheme,
		LoseTheme
	};
}

template <typename Resource, typename Identifier>
class ResourceHolder;

typedef ResourceHolder<sf::Texture, Textures::ID> TextureHolder;
typedef ResourceHolder<sf::Font, Fonts::ID> FontHolder;
typedef ResourceHolder<sf::SoundBuffer, Sounds::ID> SoundHolder;