/**
 * @file llthreadsafedisk.h
 * @brief Worker thread to read/write from/to disk
 *        in a thread safe manner
 *
 * $LicenseInfo:firstyear=2009&license=viewerlgpl$
 * Second Life Viewer Source Code
 * Copyright (C) 2020, Linden Research, Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation;
 * version 2.1 of the License only.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * Linden Research, Inc., 945 Battery Street, San Francisco, CA  94111  USA
 * $/LicenseInfo$
 */

#ifndef _LLTHREADSAFEDISKCACHE
#define _LLTHREADSAFEDISKCACHE

#include "llthreadsafequeue.h"

// todo: better name


class llThreadSafeDiskCache :
    public LLSingleton<llThreadSafeDiskCache>
{
    LLSINGLETON(llThreadSafeDiskCache);

    public:
        virtual ~llThreadSafeDiskCache();

        void cleanupSingleton() override;

        typedef std::shared_ptr<std::vector<U8>> shared_payload_t;
        typedef std::function<void(void*, shared_payload_t, bool)> vfs_callback_t;
        typedef void* vfs_callback_data_t;

        void addReadRequest(std::string filename, vfs_callback_t cb, vfs_callback_data_t cbd);

    private:
        std::thread mWorkerThread;

        // todo: better name
        struct result
        {
            U32 id;
            shared_payload_t payload;
            bool ok;
        };

        // todo: better name
        struct request
        {
            vfs_callback_t cb;
            vfs_callback_data_t cbd;
        };

        typedef std::function<result()> callable_t;
        LLThreadSafeQueue<callable_t> mInQueue;
        LLThreadSafeQueue<result> mOutQueue;

        typedef std::map<U32, request> request_map_t;
        request_map_t mRequestMap;

        std::unique_ptr<LLEventTimer> ticker;

    private:
        void requestThread();
        void perTick(/*request_map_t& rm, LLThreadSafeQueue<result>& out*/);
};

#endif // _LLTHREADSAFEDISKCACHE
