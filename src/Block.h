#pragma once

#include <SFML\Graphics\RectangleShape.hpp>
#include <SFML\Graphics\Color.hpp>
#include "Entity.h"

namespace sm
{
	class Block: public Entity
	{
	public:
		enum BlockColor
		{
			Red = 0,
			Yellow,
			Blue,
			Green,
			Grey,
			BlockColorCount
		};

		static const float SIZE;
		static const float PADDING;
		
		static BlockColor getRandomColor(void);

		Block(void);
		virtual ~Block(void);

		void activate(const BlockColor&);
		void deactivate(void);
		bool isActive(void) const { return mActive; }
		
		virtual void draw(sf::RenderTarget&, sf::RenderStates) const;

		void setColor(const BlockColor& color) { mColor = color; }
		BlockColor getColor(void) const { return mColor; }

		void copyFrom(const Block&);
		void markForDeletion(void) { mMarked = true; }
		void setMarkedForDeletion(const bool mark) { mMarked = mark; }
		bool isMarkedForDeletion(void) const { return mMarked; }
		
	private:
		static const sf::Color sColorArray[BlockColorCount];
		
		static BlockColor convertIntToColor(int);

		BlockColor mColor;
		bool mActive;
		bool mMarked;
		sf::RectangleShape mRectangle;

		void updateColor(void);
	};
}