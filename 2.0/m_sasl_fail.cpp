/*
 * InspIRCd -- Internet Relay Chat Daemon
 *
 *   Copyright (C) 2017 Adam <Adam@anope.org>
 *
 * This file is part of InspIRCd.  InspIRCd is free software: you can
 * redistribute it and/or modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation, version 2.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "inspircd.h"
#include "sasl.h"

class ModuleSASLFail : public Module
{
 public:
	void init()
	{
		Implementation eventlist[] = { I_OnEvent };
		ServerInstance->Modules->Attach(eventlist, this, sizeof(eventlist)/sizeof(Implementation));
	}

	void OnEvent(Event& event)
	{
		if (event.id == "sasl_fallback")
		{
			SASLFallback *sf = (SASLFallback *) &event;

			if (sf->params.size() <= 2)
				return;

			std::string uid = sf->params[2];

			User *user = ServerInstance->FindNick(uid);
			if (user && IS_LOCAL(user))
				user->WriteNumeric(906, "%s :SASL authentication aborted", user->nick.c_str());
		}
	}

	Version GetVersion()
	{
		return Version("Abort sasl on fallback");
	}
};

MODULE_INIT(ModuleSASLFail)
