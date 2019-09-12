/*
 * InspIRCd -- Internet Relay Chat Daemon
 *
 *   Copyright (C) 2019 Peter Powell <petpow@saberuk.com>
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

// IMPORTANT: The contents of this file are experimental and are not presently
// covered by the InspIRCd API stability guarantee.


#pragma once

#include "modules/cap.h"

namespace IRCv3
{
	namespace Replies
	{
		class Reply;
		class Fail;
		class Note;
		class Warn;
	}
}

/** Base class for standard replies. */
class IRCv3::Replies::Reply
{
 private:
	/** The name of the command for this reply. */
	std::string cmd;

	/** The event provider for this reply. */
	ClientProtocol::EventProvider evprov;

 protected:
	/** Initializes a new instance of the Reply class.
	 * @param Creator The module which created this instance.
	 * @param Cmd The name of the command to reply with.
	 */
	Reply(Module* Creator, const std::string& Cmd)
		: cmd(Cmd)
		, evprov(Creator, Cmd)
	{
	}

 public:
	/**
	 * Sends a standard reply to the specified user.
	 * @param user The user to send the reply to.
	 * @param command The command that the reply relates to.
	 * @param code A machine readable code for this reply.
	 * @param description A human readable description of this reply.
	 */
	void Send(LocalUser* user, Command* command, const std::string& code, const std::string& description)
	{
		ClientProtocol::Message msg(cmd.c_str(), ServerInstance->Config->ServerName);
		msg.PushParamRef(command->name);
		msg.PushParam(code);
		msg.PushParam(description);

		ClientProtocol::Event ev(evprov, msg);
		user->Send(ev);
	}

	/**
	 * Sends a standard reply to the specified user if they have the specified cap
	 * or a notice if they do not.s
	 * @param user The user to send the reply to.
	 * @param command The command that the reply relates to.
	 * @param code A machine readable code for this reply.
	 * @param description A human readable description of this reply.
	 */
	void SendIfCap(LocalUser* user, const Cap::Capability& cap, Command* command, const std::string& code, const std::string& description)
	{
		if (cap.get(user))
			Send(user, command, code, description);
		else
			user->WriteNotice(InspIRCd::Format("*** %s: %s", command->name.c_str(), description.c_str()));
	}
};

/** Sends a FAIL standard reply. */
class IRCv3::Replies::Fail
	: public IRCv3::Replies::Reply
{
public:
	/** Initializes a new instance of the Fail class.
	 * @param Creator The module which created this instance.
	 */
	Fail(Module* Creator)
		: Reply(Creator, "FAIL")
	{
	}
};

/** Sends a NOTE standard reply. */
class IRCv3::Replies::Note
	: public IRCv3::Replies::Reply
{
public:
	/** Initializes a new instance of the Note class.
	 * @param Creator The module which created this instance.
	 */
	Note(Module* Creator)
		: Reply(Creator, "NOTE")
	{
	}
};

/** Sends a WARN standard reply. */
class IRCv3::Replies::Warn
	: public IRCv3::Replies::Reply
{
public:
	/** Initializes a new instance of the Warn class.
	 * @param Creator The module which created this instance.
	 */
	Warn(Module* Creator)
		: Reply(Creator, "WARN")
	{
	}
};
