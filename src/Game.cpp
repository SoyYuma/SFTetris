#include "Game.h"

#include <Windows.h>
#include <SFML\System\Time.hpp>
#include "State.h"
#include "Action.h"
#include "Logger.h"
#include "MathSystem.h"
#include "InputSystem.h"
#include "AudioSystem.h"
#include "StateMachine.h"

namespace sm
{
	boost::shared_ptr<Game> Game::sInstance;
	
	const unsigned int Game::FRAMERATE = 60;
	const sf::VideoMode Game::VIDEO_MODE = sf::VideoMode(325, 400);
	const std::string Game::TITLE = std::string("Simple+Fast TETRIS");
	const unsigned int Game::STYLE = sf::Style::Titlebar | sf::Style::Close;

	Game::Game(void): RenderWindow(VIDEO_MODE, TITLE, STYLE), mStateMachine(), mLogger(new Logger()),
		mCurrentActionList(0), mDeltaClock(), mDeltaTime(), mPaused(false)
	{
		setFramerateLimit(FRAMERATE);

		mMath = boost::shared_ptr<MathSystem>(new MathSystem());
		mInput = boost::shared_ptr<InputSystem>(new InputSystem());
		mAudio = boost::shared_ptr<AudioSystem>(new AudioSystem());

		mStateMachine = boost::shared_ptr<StateMachine>(new StateMachine("GameSM"));

		mActions[0] = ActionContainer();
		mActions[1] = ActionContainer();
	}

	Game::~Game(void)
	{
		if(mMath)
		{
			mMath->quit();
		}
		if(mInput)
		{
			mInput->quit();
		}
		if(mAudio)
		{
			mAudio->quit();
		}
	}

	boost::shared_ptr<Game> Game::instance(void)
	{
		if(!sInstance)
		{
			sInstance = boost::shared_ptr<Game>(new Game());
		}

		return sInstance;
	}

	void Game::addState(const boost::shared_ptr<State>& state)
	{
		mStateMachine->addState(state);
	}

	void Game::addRuleForAll(const std::string& action, const std::string& state)
	{
		mStateMachine->addRuleForAll(action, state);
	}

	void Game::addRule(const std::string& action, const std::string& origin,
		const std::string& destination)
	{
		mStateMachine->addRule(action, origin, destination);
	}

	boost::shared_ptr<InputSystem> Game::getInput(void)
	{
		return mInput;
	}

	boost::shared_ptr<MathSystem> Game::getMath(void)
	{
		return mMath;
	}

	boost::shared_ptr<AudioSystem> Game::getAudio(void)
	{
		return mAudio;
	}

	boost::shared_ptr<Logger> Game::getLogger(void)
	{
		return mLogger;
	}
	
	void Game::addAction(const boost::shared_ptr<Action>& action)
	{
		mActions[(mCurrentActionList + 1) % 2].push_back(boost::shared_ptr<Action>(action));
	}

	const Game::ActionContainer& Game::getActions(void) const
	{
		return mActions[mCurrentActionList];
	}

	sf::Time Game::getDeltaTime(void) const
	{
		return mDeltaTime;
	}

	void Game::init(void)
	{
		mMath->init();
		mInput->init();
		mAudio->init();
	}

	void Game::loop(void)
	{
		while(isOpen())
		{
			// system updates
			getMath()->update();
			getInput()->update();
			getAudio()->update();
			
			clear(sf::Color(0x20, 0x20, 0x20, 255));
			if(!mPaused)
			{
				mStateMachine->update();

				mActions[mCurrentActionList].clear();
				mCurrentActionList = (mCurrentActionList+1) % 2;
			}
			draw(*mStateMachine.get());
			display();

			mDeltaTime = mDeltaClock.restart();
		}

		mStateMachine->quit();
	}

	void Game::pause(void)
	{
		mPaused = true;
	}

	void Game::unpause(void)
	{
		mPaused = false;
	}
}