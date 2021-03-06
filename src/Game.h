#pragma once

#include <string>
#include <vector>
#include <boost\shared_ptr.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

namespace sf
{
	class Clock;
	class Time;
}

namespace sm
{
	class State;
	class StateMachine;
	class Action;
	class Logger;
	class InputSystem;
	class MathSystem;
	class AudioSystem;

	class Game : public sf::RenderWindow
	{
	public:
		typedef std::vector<boost::shared_ptr<Action>> ActionContainer;

		static boost::shared_ptr<Game> instance();

		~Game(void);

		void addState(const boost::shared_ptr<State>&);
		void addRuleForAll(const std::string&, const std::string&);
		void addRule(const std::string&, const std::string&, const std::string&);

		boost::shared_ptr<InputSystem> getInput(void);
		boost::shared_ptr<MathSystem> getMath(void);
		boost::shared_ptr<AudioSystem> getAudio(void);
		boost::shared_ptr<Logger> getLogger(void);

		void addAction(const boost::shared_ptr<Action>&);
		const ActionContainer& getActions(void) const;

		sf::Time getDeltaTime(void) const;

		void init(void);
		void loop(void);

		void pause(void);
		void unpause(void);
		bool isPaused(void) const { return mPaused; }

	private:
		static boost::shared_ptr<Game> sInstance;

		static const unsigned int FRAMERATE;
		static const sf::VideoMode VIDEO_MODE;
		static const std::string TITLE;
		static const unsigned int STYLE;

		boost::shared_ptr<StateMachine> mStateMachine;
		boost::shared_ptr<Logger> mLogger;
		boost::shared_ptr<MathSystem> mMath;
		boost::shared_ptr<InputSystem> mInput;
		boost::shared_ptr<AudioSystem> mAudio;
		ActionContainer mActions[2];
		unsigned int mCurrentActionList;
		sf::Clock mDeltaClock;
		sf::Time mDeltaTime;
		bool mPaused;

		Game(void);
		Game(const Game&) {}
		Game& operator=(const Game&) {}
	};
}