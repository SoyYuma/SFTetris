#include "MainState.h"

#include "Game.h"
#include "Board.h"
#include "Piece.h"
#include "Action.h"
#include "Logger.h"
#include "MathSystem.h"
#include "AudioSystem.h"
#include "InputSystem.h"

namespace sm
{
	const sf::Time MainState::TickingTimes[5] = {
		                                         sf::seconds(1.f),
			                                     sf::seconds(0.75f),
			                                     sf::seconds(0.65f),
			                                     sf::seconds(0.5f),
			                                     sf::seconds(0.4f)
	                                            };

	MainState::MainState(void): State("game"), mClock(), mTime(), mTimeScale(0), 
		mBoard(), mPreviewBoard(), mCurrentPiece(), mNextPiece(),
		mMusicPiece()
	{
		mTime = TickingTimes[mTimeScale];
	}

	void MainState::init(void)
	{
		mClock = Game::instance()->getMath()->createTimer();

		Game::instance()->getLogger()->getBuffer() << "MainState::init()";
		Game::instance()->getLogger()->debug(5);

		mBoard = boost::shared_ptr<Board>(new Board(21, 11));
		mBoard->setPosition(20.f, 25.f);
		addEntity(boost::shared_ptr<Entity>(mBoard));

		mPreviewBoard = boost::shared_ptr<Board>(new Board(5, 5));
		mPreviewBoard->setPosition(225.f, 25.f);
		addEntity(boost::shared_ptr<Entity>(mPreviewBoard));

		mCurrentPiece = boost::shared_ptr<Piece>(new Piece(mBoard, 0, 5));
		mNextPiece = boost::shared_ptr<Piece>(new Piece(mPreviewBoard, 2, 2));

		mMusicPiece = Game::instance()->getAudio()->createMusicPiece();
		Game::instance()->getAudio()->getMusicPiece(mMusicPiece)->openFromFile("assets/main_theme.ogg");
		Game::instance()->getAudio()->getMusicPiece(mMusicPiece)->setLoop(true);
		Game::instance()->getAudio()->playMusicPiece(mMusicPiece);
	}

	void MainState::update(void)
	{
		if(Game::instance()->getMath()->getTimeOfTimer(mClock) > mTime)
		{
			Game::instance()->addAction(boost::shared_ptr<Action>(new Action("tick")));
			
			mTime = TickingTimes[mTimeScale] 
				- (Game::instance()->getMath()->getTimeOfTimer(mClock) - mTime);
			
			Game::instance()->getMath()->restartTimer(mClock);

			// with every tick check block deletion because of colors
			mCurrentPiece->turnOff();
			mBoard->checkColors();
			mCurrentPiece->turnOn();
		}

		if(!mCurrentPiece->isStuck())
		{
			mCurrentPiece->update();
		}
		else
		{
			// prepare preview piece to be a current piece
			mCurrentPiece = mNextPiece;
			mCurrentPiece->setBoard(mBoard);
			mCurrentPiece->setRow(0);
			mCurrentPiece->setColumn(5);
			
			// create a new preview piece
			mPreviewBoard->resetBlocks();
			mNextPiece.reset(new Piece(mPreviewBoard, 2, 2));

			// check for game over
			if(mCurrentPiece->checkCollision())
			{
				Game::instance()->getLogger()->getBuffer() << "GAME OVER!";
				Game::instance()->getLogger()->debug(5);
			}
			// check for block deletion
			else
			{
				mCurrentPiece->turnOff();
				mBoard->checkHorizontal();
				mCurrentPiece->turnOn();
			}
		}

		if(Game::instance()->getInput()->isKeyReleased(InputSystem::Keyboard::Escape))
		{
			Game::instance()->addAction(boost::shared_ptr<Action>(new Action("gomenu")));
		}

		State::update();
	}

	void MainState::quit(void)
	{
		Game::instance()->getLogger()->getBuffer() << "MainState::quit()";
		Game::instance()->getLogger()->debug(5);
		Game::instance()->getAudio()->stopMusic();
	}
}	