/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "im.h"

bool_t
xdr_User (XDR *xdrs, User *objp)
{
	register int32_t *buf;

	 if (!xdr_u_int (xdrs, &objp->id))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->name, 32))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->nick, 32))
		 return FALSE;
	 if (!xdr_bool (xdrs, &objp->onlineStatus))
		 return FALSE;
	 if (!xdr_bool (xdrs, &objp->isFriend))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_Message (XDR *xdrs, Message *objp)
{
	register int32_t *buf;

	 if (!xdr_User (xdrs, &objp->sender))
		 return FALSE;
	 if (!xdr_User (xdrs, &objp->recipient))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->msg, ~0))
		 return FALSE;
	 if (!xdr_bool (xdrs, &objp->read))
		 return FALSE;
	 if (!xdr_u_int (xdrs, &objp->date))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_Session (XDR *xdrs, Session *objp)
{
	register int32_t *buf;

	 if (!xdr_int (xdrs, &objp->id))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_Args_login (XDR *xdrs, Args_login *objp)
{
	register int32_t *buf;

	 if (!xdr_string (xdrs, &objp->name, 32))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->passwd, 32))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_Args_sendMessage (XDR *xdrs, Args_sendMessage *objp)
{
	register int32_t *buf;

	 if (!xdr_Session (xdrs, &objp->session))
		 return FALSE;
	 if (!xdr_u_int (xdrs, &objp->recipientId))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->msg, ~0))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_Args_addUserToRoster (XDR *xdrs, Args_addUserToRoster *objp)
{
	register int32_t *buf;

	 if (!xdr_Session (xdrs, &objp->session))
		 return FALSE;
	 if (!xdr_u_int (xdrs, &objp->userId))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_Args_deleteUserFromRoster (XDR *xdrs, Args_deleteUserFromRoster *objp)
{
	register int32_t *buf;

	 if (!xdr_Session (xdrs, &objp->session))
		 return FALSE;
	 if (!xdr_u_int (xdrs, &objp->userId))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_Args_findUser (XDR *xdrs, Args_findUser *objp)
{
	register int32_t *buf;

	 if (!xdr_Session (xdrs, &objp->session))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->name, 32))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->nick, 32))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_Args_register (XDR *xdrs, Args_register *objp)
{
	register int32_t *buf;

	 if (!xdr_string (xdrs, &objp->name, 32))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->nick, 32))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->passwd, 32))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_Status (XDR *xdrs, Status *objp)
{
	register int32_t *buf;

	 if (!xdr_bool (xdrs, &objp->stat))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->descr, ~0))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_Result_login (XDR *xdrs, Result_login *objp)
{
	register int32_t *buf;

	 if (!xdr_Status (xdrs, &objp->status))
		 return FALSE;
	 if (!xdr_Session (xdrs, &objp->session))
		 return FALSE;
	 if (!xdr_User (xdrs, &objp->user))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_Result_getUserRoster (XDR *xdrs, Result_getUserRoster *objp)
{
	register int32_t *buf;

	 if (!xdr_Status (xdrs, &objp->status))
		 return FALSE;
	 if (!xdr_array (xdrs, (char **)&objp->roster.roster_val, (u_int *) &objp->roster.roster_len, ~0,
		sizeof (User), (xdrproc_t) xdr_User))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_Result_getMessagesHistory (XDR *xdrs, Result_getMessagesHistory *objp)
{
	register int32_t *buf;

	 if (!xdr_Status (xdrs, &objp->status))
		 return FALSE;
	 if (!xdr_array (xdrs, (char **)&objp->messages.messages_val, (u_int *) &objp->messages.messages_len, ~0,
		sizeof (Message), (xdrproc_t) xdr_Message))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_Result_getUnreadMessages (XDR *xdrs, Result_getUnreadMessages *objp)
{
	register int32_t *buf;

	 if (!xdr_Status (xdrs, &objp->status))
		 return FALSE;
	 if (!xdr_array (xdrs, (char **)&objp->messages.messages_val, (u_int *) &objp->messages.messages_len, ~0,
		sizeof (Message), (xdrproc_t) xdr_Message))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_Result_findUser (XDR *xdrs, Result_findUser *objp)
{
	register int32_t *buf;

	 if (!xdr_Status (xdrs, &objp->status))
		 return FALSE;
	 if (!xdr_array (xdrs, (char **)&objp->users.users_val, (u_int *) &objp->users.users_len, ~0,
		sizeof (User), (xdrproc_t) xdr_User))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_Result_getFriendshipRequests (XDR *xdrs, Result_getFriendshipRequests *objp)
{
	register int32_t *buf;

	 if (!xdr_Status (xdrs, &objp->status))
		 return FALSE;
	 if (!xdr_array (xdrs, (char **)&objp->users.users_val, (u_int *) &objp->users.users_len, ~0,
		sizeof (User), (xdrproc_t) xdr_User))
		 return FALSE;
	return TRUE;
}
