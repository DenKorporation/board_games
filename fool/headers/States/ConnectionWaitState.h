#pragma once

#include "Engine/State.h"
#include "GUI/Container.h"
#include "GUI/Label.h"
#include "ServerService.h"

#include <SFML/Graphics/Sprite.hpp>

#include <thread>
#include <mutex>
#include <future>

class ConnectionWaitState : public State
{
public:
	ConnectionWaitState(StateStack &stack, Context context);
	~ConnectionWaitState();

	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event &event);

private:
	void cancelAction();
	void initQuery();
	void backgroundAction(std::future<void> futureObj);

private:
	SceneNode mSceneGraph;
	GUI::Container *mGUIContainer;

	GUI::Label *mHeaderLabel;
	GUI::Label *mNameLabel;
	GUI::Label *mIdLabel;

	std::unique_ptr<ServerService> mService;
	bool mIsConnected;
	bool mIsGameStarted;

	std::mutex mCommonMutex;

	std::promise<void> mPromise;
	std::future<void> mFutureObj;

	std::thread mInitThread;
	std::thread mBackgroundThread;
	std::mutex mSendMutex;
	std::mutex mReceiveMutex;
	std::mutex mConnectMutex;

	std::thread mCancelThread;
	std::mutex mCancelMutex;
	bool mCancelThreadRunning;
};