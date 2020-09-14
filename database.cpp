// Copyright (c) 2015-2020 Vector 35 Inc
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
#include <cstring>
#include "binaryninjaapi.h"

using namespace BinaryNinja;
using namespace Json;
using namespace std;


Snapshot::Snapshot(BNSnapshot* snapshot)
{
	m_object = snapshot;
}


int64_t Snapshot::GetId()
{
	return BNGetSnapshotId(m_object);
}


std::string Snapshot::GetName()
{
	char* cstr = BNGetSnapshotName(m_object);
	std::string str{cstr};
	BNFreeString(cstr);
	return str;
}


bool Snapshot::IsAutoSave()
{
	return BNIsSnapshotAutoSave(m_object);
}


Ref<Snapshot> Snapshot::GetParent()
{
	BNSnapshot* snap = BNGetSnapshotParent(m_object);
	if (snap == nullptr)
		return nullptr;
	return new Snapshot(snap);
}


DataBuffer Snapshot::GetFileContents()
{
	return DataBuffer(BNGetSnapshotFileContents(m_object));
}


vector<UndoEntry> Snapshot::GetUndoEntries()
{
	size_t numEntries;
	BNUndoEntry* entries = BNGetSnapshotUndoEntries(m_object, &numEntries);

	vector<UndoEntry> result;
	result.reserve(numEntries);
	for (size_t i = 0; i < numEntries; i++)
	{
		UndoEntry temp;
		temp.timestamp = entries[i].timestamp;
		temp.hash = entries[i].hash;
		temp.user = new User(BNNewUserReference(entries[i].user));
		size_t actionCount = entries[i].actionCount;
		for (size_t actionIndex = 0; actionIndex < actionCount; actionIndex++)
		{
			temp.actions.emplace_back(entries[i].actions[actionIndex]);
		}
		result.push_back(temp);
	}

	BNFreeUndoEntries(entries, numEntries);
	return result;
}


Database::Database(BNDatabase* database)
{
	m_object = database;
}


Ref<Snapshot> Database::GetSnapshot(int64_t id)
{
	BNSnapshot* snap = BNGetDatabaseSnapshot(m_object, id);
	if (snap == nullptr)
		return nullptr;
	return new Snapshot(snap);
}


Ref<Snapshot> Database::GetCurrentSnapshot()
{
	BNSnapshot* snap = BNGetDatabaseCurrentSnapshot(m_object);
	if (snap == nullptr)
		return nullptr;
	return new Snapshot(snap);
}
