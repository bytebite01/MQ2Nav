/*
 * MacroQuest: The extension platform for EverQuest
 * Copyright (C) 2002-2023 MacroQuest Authors
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License, version 2, as published by
 * the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#pragma once

#include "../src/common/Common.h"

#include <mq/base/Common.h>
#include <mq/base/Threading.h>
#include <mq/base/BuildInfo.h>

#include <functional>

#include <routing/PostOffice.h>


namespace mq {

	namespace pipeclient {

		void NotifyIsForegroundWindow(bool isForeground);
		void RequestActivateWindow(HWND hWnd, bool sendMessage = true);

	} // namespace pipeclient

	using namespace postoffice;
	

	class AppPostOffice : public PostOffice
	{
	private:

		struct ClientIdentification;

		std::unordered_map<uint32_t, ClientIdentification> m_identities;
		ci_unordered::map<std::string, uint32_t> m_names;

		class PipeEventsHandler : public NamedPipeEvents
		{
		public:
			PipeEventsHandler(AppPostOffice* postOffice);

			virtual void OnIncomingMessage(PipeMessagePtr&& message);

			virtual void OnClientConnected() override;

		private:
			AppPostOffice* m_postOffice;
		};

	public:

		AppPostOffice();
			

		static void RoutingFailed(
			const proto::routing::Envelope& envelope,
			int status,
			PipeMessagePtr&& message,
			const PipeMessageResponseCb& callback);

		std::unordered_map<const std::string, std::unique_ptr<postoffice::Mailbox>>::iterator FindMailbox(
			const proto::routing::Address& address,
			const std::unordered_map<const std::string, std::unique_ptr<postoffice::Mailbox>>::iterator& from);

		void RouteMessage(PipeMessagePtr&& message, const PipeMessageResponseCb& callback);

		void ProcessPipeClient();

		void NotifyIsForegroundWindow(bool isForeground);

		void RequestActivateWindow(HWND hWnd, bool sendMessage);

		void SetGameStatePostOffice(DWORD GameState);
		void Initialize();

		void Shutdown();

	private:
		ProtoPipeClient m_pipeClient;
		Dropbox m_clientDropbox;
		DWORD m_launcherProcessID;

		static void StopPipeClient();
		
	};

	PostOffice& postoffice::GetPostOffice();

	namespace pipeclient {
		void NotifyIsForegroundWindow(bool isForeground);
		void RequestActivateWindow(HWND hWnd, bool sendMessage);
		void InitializePostOffice();
		void ShutdownPostOffice();
		void PulsePostOffice();
		void SetGameStatePostOffice(DWORD GameState);
	}

} // namespace mq
