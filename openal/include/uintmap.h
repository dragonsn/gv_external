#ifndef AL_UINTMAP_H
#define AL_UINTMAP_H

#include "AL/al.h"
#include "rwlock.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct UIntMap {
    struct pair{
        ALuint key;
        ALvoid *value;
    } ;
	pair *array;
    ALsizei size;
    ALsizei maxsize;
    ALsizei limit;
    RWLock lock;
} UIntMap;
#define UINTMAP_STATIC_INITIALIZE_N(_n) { NULL, 0, 0, (_n), RWLOCK_STATIC_INITIALIZE }
#define UINTMAP_STATIC_INITIALIZE UINTMAP_STATIC_INITIALIZE_N(~0)

void InitUIntMap(UIntMap *map, ALsizei limit);
void ResetUIntMap(UIntMap *map);
ALenum InsertUIntMapEntry(UIntMap *map, ALuint key, ALvoid *value);
ALvoid *RemoveUIntMapKey(UIntMap *map, ALuint key);
ALvoid *LookupUIntMapKey(UIntMap *map, ALuint key);

inline void LockUIntMapRead(UIntMap *map)
{ ReadLock(&map->lock); }
inline void UnlockUIntMapRead(UIntMap *map)
{ ReadUnlock(&map->lock); }
inline void LockUIntMapWrite(UIntMap *map)
{ WriteLock(&map->lock); }
inline void UnlockUIntMapWrite(UIntMap *map)
{ WriteUnlock(&map->lock); }

#ifdef __cplusplus
}
#endif

#endif /* AL_UINTMAP_H */
