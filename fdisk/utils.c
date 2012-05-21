/*
 *  Copyright (C) 2012 Davidlohr Bueso <dave@gnu.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "common.h"
#include "fdisk.h"

/**
 * fdisk_new_context:
 *
 * Returns: newly allocated fdisk context
 */
struct fdisk_context *fdisk_new_context_from_filename(const char *fname)
{
	int fd, errsv = 0;
	struct fdisk_context *cxt = NULL;

	/*
	 * Attempt to open the device with r-w permissions
	 * by default, otherwise try read-only.
	 */
	if ((fd = open(fname, O_RDWR)) < 0)
		if ((fd = open(fname, O_RDONLY)) < 0)
			return NULL;

	cxt = calloc(1, sizeof(*cxt));
	if (!cxt)
		goto fail;

	cxt->dev_fd = fd;
	cxt->dev_path = strdup(fname);
	if (!cxt->dev_path)
		goto fail;

	return cxt;
fail:
	errsv = errno;
	fdisk_free_context(cxt);
	errno = errsv;
	return NULL;
}

/**
 * fdisk_free_context:
 * @cxt: fdisk context
 *
 * Deallocates context struct.
 */
void fdisk_free_context(struct fdisk_context *cxt)
{
	if (!cxt)
		return;

	close(cxt->dev_fd);
	free(cxt->dev_path);
	free(cxt);
}