#ifndef AL_BACKENDS_BASE_H
#define AL_BACKENDS_BASE_H

#include "../../OpenAL32/Include/alMain.h"
#include "../../include/threads.h"


struct ALCbackendVtable;

typedef struct ALCbackend {
    const struct ALCbackendVtable *vtbl;

    ALCdevice *mDevice;

    almtx_t mMutex;
} ALCbackend;

void ALCbackend_Construct(ALCbackend *self, ALCdevice *device);
void ALCbackend_Destruct(ALCbackend *self);
ALCboolean ALCbackend_reset(ALCbackend *self);
ALCenum ALCbackend_captureSamples(ALCbackend *self, void *buffer, ALCuint samples);
ALCuint ALCbackend_availableSamples(ALCbackend *self);
ALint64 ALCbackend_getLatency(ALCbackend *self);
void ALCbackend_lock(ALCbackend *self);
void ALCbackend_unlock(ALCbackend *self);

struct ALCbackendVtable {
    void (*const Destruct)(ALCbackend*);

    ALCenum (*const open)(ALCbackend*, const ALCchar*);
    void (*const close)(ALCbackend*);

    ALCboolean (*const reset)(ALCbackend*);
    ALCboolean (*const start)(ALCbackend*);
    void (*const stop)(ALCbackend*);

    ALCenum (*const captureSamples)(ALCbackend*, void*, ALCuint);
    ALCuint (*const availableSamples)(ALCbackend*);

    ALint64 (*const getLatency)(ALCbackend*);

    void (*const lock)(ALCbackend*);
    void (*const unlock)(ALCbackend*);

    void (*const Delete)(void*);
};

#define DEFINE_ALCBACKEND_VTABLE(T)                                           \
DECLARE_THUNK(T, ALCbackend, void, Destruct)                                  \
DECLARE_THUNK1(T, ALCbackend, ALCenum, open, const ALCchar*)                  \
DECLARE_THUNK(T, ALCbackend, void, close)                                     \
DECLARE_THUNK(T, ALCbackend, ALCboolean, reset)                               \
DECLARE_THUNK(T, ALCbackend, ALCboolean, start)                               \
DECLARE_THUNK(T, ALCbackend, void, stop)                                      \
DECLARE_THUNK2(T, ALCbackend, ALCenum, captureSamples, void*, ALCuint)        \
DECLARE_THUNK(T, ALCbackend, ALCuint, availableSamples)                       \
DECLARE_THUNK(T, ALCbackend, ALint64, getLatency)                             \
DECLARE_THUNK(T, ALCbackend, void, lock)                                      \
DECLARE_THUNK(T, ALCbackend, void, unlock)                                    \
static void T##_ALCbackend_Delete(void *ptr)                                  \
{ T##_Delete(STATIC_UPCAST(T, ALCbackend, (ALCbackend*)ptr)); }               \
                                                                              \
static const struct ALCbackendVtable T##_ALCbackend_vtable = {                \
    T##_ALCbackend_Destruct,                                                  \
                                                                              \
    T##_ALCbackend_open,                                                      \
    T##_ALCbackend_close,                                                     \
    T##_ALCbackend_reset,                                                     \
    T##_ALCbackend_start,                                                     \
    T##_ALCbackend_stop,                                                      \
    T##_ALCbackend_captureSamples,                                            \
    T##_ALCbackend_availableSamples,                                          \
    T##_ALCbackend_getLatency,                                                \
    T##_ALCbackend_lock,                                                      \
    T##_ALCbackend_unlock,                                                    \
                                                                              \
    T##_ALCbackend_Delete,                                                    \
}


typedef enum ALCbackend_Type {
    ALCbackend_Playback,
    ALCbackend_Capture,
    ALCbackend_Loopback
} ALCbackend_Type;


struct ALCbackendFactoryVtable;

typedef struct ALCbackendFactory {
    const struct ALCbackendFactoryVtable *vtbl;
} ALCbackendFactory;

void ALCbackendFactory_deinit(ALCbackendFactory *self);

struct ALCbackendFactoryVtable {
    ALCboolean (*const init)(ALCbackendFactory *self);
    void (*const deinit)(ALCbackendFactory *self);

    ALCboolean (*const querySupport)(ALCbackendFactory *self, ALCbackend_Type type);

    void (*const probe)(ALCbackendFactory *self, enum DevProbe type);

    ALCbackend* (*const createBackend)(ALCbackendFactory *self, ALCdevice *device, ALCbackend_Type type);
};

#define DEFINE_ALCBACKENDFACTORY_VTABLE(T)                                    \
DECLARE_THUNK(T, ALCbackendFactory, ALCboolean, init)                         \
DECLARE_THUNK(T, ALCbackendFactory, void, deinit)                             \
DECLARE_THUNK1(T, ALCbackendFactory, ALCboolean, querySupport, ALCbackend_Type) \
DECLARE_THUNK1(T, ALCbackendFactory, void, probe, enum DevProbe)              \
DECLARE_THUNK2(T, ALCbackendFactory, ALCbackend*, createBackend, ALCdevice*, ALCbackend_Type) \
                                                                              \
static const struct ALCbackendFactoryVtable T##_ALCbackendFactory_vtable = {  \
    T##_ALCbackendFactory_init,                                               \
    T##_ALCbackendFactory_deinit,                                             \
    T##_ALCbackendFactory_querySupport,                                       \
    T##_ALCbackendFactory_probe,                                              \
    T##_ALCbackendFactory_createBackend,                                      \
}


ALCbackendFactory *ALCpulseBackendFactory_getFactory(void);
ALCbackendFactory *ALCalsaBackendFactory_getFactory(void);
ALCbackendFactory *ALCossBackendFactory_getFactory(void);
ALCbackendFactory *ALCmmdevBackendFactory_getFactory(void);
ALCbackendFactory *ALCdsoundBackendFactory_getFactory(void);
ALCbackendFactory *ALCnullBackendFactory_getFactory(void);
ALCbackendFactory *ALCloopbackFactory_getFactory(void);

ALCbackend *create_backend_wrapper(ALCdevice *device, const BackendFuncs *funcs, ALCbackend_Type type);

#endif /* AL_BACKENDS_BASE_H */
