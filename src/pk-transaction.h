/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*-
 *
 * Copyright (C) 2008-2011 Richard Hughes <richard@hughsie.com>
 *
 * Licensed under the GNU General Public License Version 2
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef __PK_TRANSACTION_H
#define __PK_TRANSACTION_H

#include <glib-object.h>
#include <packagekit-glib2/pk-enum.h>
#include <packagekit-glib2/pk-results.h>

#include "pk-backend.h"

G_BEGIN_DECLS

#define PK_TYPE_TRANSACTION		(pk_transaction_get_type ())
#define PK_TRANSACTION(o)		(G_TYPE_CHECK_INSTANCE_CAST ((o), PK_TYPE_TRANSACTION, PkTransaction))
#define PK_IS_TRANSACTION(o)	 	(G_TYPE_CHECK_INSTANCE_TYPE ((o), PK_TYPE_TRANSACTION))
#define PK_TRANSACTION_ERROR		(pk_transaction_error_quark ())

typedef struct PkTransactionPrivate PkTransactionPrivate;

typedef struct
{
	 GObject		 parent;
	 PkTransactionPrivate	*priv;
} PkTransaction;

typedef struct
{
	GObjectClass	parent_class;
} PkTransactionClass;

/* these have to be kept in order */
typedef enum {
	PK_TRANSACTION_STATE_NEW,
	PK_TRANSACTION_STATE_WAITING_FOR_AUTH,
	PK_TRANSACTION_STATE_READY,
	PK_TRANSACTION_STATE_RUNNING,
	PK_TRANSACTION_STATE_FINISHED,
	PK_TRANSACTION_STATE_ERROR,
	PK_TRANSACTION_STATE_UNKNOWN
} PkTransactionState;

GQuark		 pk_transaction_error_quark			(void);
GType		 pk_transaction_get_type			(void);
PkTransaction	*pk_transaction_new				(GKeyFile		*conf,
								 GDBusNodeInfo	*introspection);

/* go go go! */
gboolean	 pk_transaction_run				(PkTransaction	*transaction)
								 G_GNUC_WARN_UNUSED_RESULT;
/* internal status */
void		 pk_transaction_cancel_bg			(PkTransaction	*transaction);
gboolean	 pk_transaction_get_background			(PkTransaction	*transaction);
PkRoleEnum	 pk_transaction_get_role			(PkTransaction	*transaction);
guint		 pk_transaction_get_uid				(PkTransaction	*transaction);
GKeyFile	*pk_transaction_get_conf			(PkTransaction	*transaction);
void		 pk_transaction_set_backend			(PkTransaction	*transaction,
								 PkBackend	*backend);
PkBackendJob	*pk_transaction_get_backend_job 		(PkTransaction	*transaction);
PkResults	*pk_transaction_get_results			(PkTransaction	*transaction);
gchar		**pk_transaction_get_package_ids		(PkTransaction	*transaction);
PkBitfield	 pk_transaction_get_transaction_flags		(PkTransaction	*transaction);
void		 pk_transaction_set_package_ids			(PkTransaction	*transaction,
								 gchar		**package_ids);
gchar		**pk_transaction_get_values			(PkTransaction	*transaction);
gchar		**pk_transaction_get_full_paths			(PkTransaction	*transaction);
void		 pk_transaction_set_full_paths			(PkTransaction	*transaction,
								 gchar		**full_paths);
PkTransactionState pk_transaction_get_state			(PkTransaction	*transaction);
void		 pk_transaction_set_state			(PkTransaction	*transaction,
								 PkTransactionState state);
const gchar	*pk_transaction_state_to_string			(PkTransactionState state);
const gchar	*pk_transaction_get_tid				(PkTransaction	*transaction);
gboolean	 pk_transaction_is_exclusive			(PkTransaction	*transaction);
void		 pk_transaction_set_supported_roles		(PkTransaction	*transaction,
								 GPtrArray	*plugins);
void		 pk_transaction_signals_reset			(PkTransaction	*transaction,
								 PkBackendJob	*job);
gboolean	 pk_transaction_is_finished_with_lock_required	(PkTransaction *transaction);
void		 pk_transaction_reset_after_lock_error		(PkTransaction *transaction);
void		 pk_transaction_make_exclusive			(PkTransaction *transaction);
void		 pk_transaction_skip_auth_checks		(PkTransaction *transaction,
								 gboolean skip_checks);

G_END_DECLS

#endif /* __PK_TRANSACTION_H */
