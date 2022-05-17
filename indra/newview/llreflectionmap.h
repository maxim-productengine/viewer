/**
 * @file llreflectionmap.h
 * @brief LLReflectionMap class declaration
 *
 * $LicenseInfo:firstyear=2022&license=viewerlgpl$
 * Second Life Viewer Source Code
 * Copyright (C) 2022, Linden Research, Inc.
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

#pragma once

#include "llcubemaparray.h"
#include "llmemory.h"

class LLSpatialGroup;
class LLViewerObject;

class alignas(16) LLReflectionMap : public LLRefCount
{
    LL_ALIGN_NEW
public:
    // allocate an environment map of the given resolution 
    LLReflectionMap();

    // update this environment map
    // resolution - size of cube map to generate
    void update(U32 resolution, U32 face);

    // return true if this probe should update *now*
    bool shouldUpdate();

    // Mark this reflection map as needing an update (resets last update time, so spamming this call will cause a cube map to never update)
    void dirty();

    // for volume partition probes, try to place this probe in the best spot
    void autoAdjustOrigin();

    // return true if given Reflection Map's influence volume intersect's with this one's
    bool intersects(LLReflectionMap* other);

    // point at which environment map was last generated from (in agent space)
    LLVector4a mOrigin;
    
    // distance from viewer camera
    F32 mDistance;

    // radius of this probe's affected area
    F32 mRadius = 16.f;

    // last time this probe was updated (or when its update timer got reset)
    F32 mLastUpdateTime = 0.f;

    // last time this probe was bound for rendering
    F32 mLastBindTime = 0.f;

    // cube map used to sample this environment map
    LLPointer<LLCubeMapArray> mCubeArray;
    S32 mCubeIndex = -1; // index into cube map array or -1 if not currently stored in cube map array

    // index into array packed by LLReflectionMapManager::getReflectionMaps
    // WARNING -- only valid immediately after call to getReflectionMaps
    S32 mProbeIndex = -1;

    // set of any LLReflectionMaps that intersect this map (maintained by LLReflectionMapManager
    std::vector<LLReflectionMap*> mNeighbors;

    // spatial group this probe is tracking (if any)
    LLSpatialGroup* mGroup = nullptr;

    // viewer object this probe is tracking (if any)
    LLViewerObject* mViewerObject = nullptr;

    bool mDirty = true;
};
