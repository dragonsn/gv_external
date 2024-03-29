/**
 * OpenAL cross platform audio library
 * Copyright (C) 2013 by Anis A. Hireche
 * This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, write to the
 *  Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 *  Boston, MA  02111-1307, USA.
 * Or go to http://www.gnu.org/copyleft/lgpl.html
 */

//#include <stdlib.h>
//
//#include "../config.h"
//#include "alError.h"
//#include "include/alMain.h"
//#include "alAuxEffectSlot.h"
//#include "alu.h"


typedef struct ALcompressorState {
    DERIVE_FROM_TYPE(ALeffectState);

    /* Effect gains for each channel */
    ALfloat Gain[MaxChannels];

    /* Effect parameters */
    ALboolean Enabled;
    ALfloat AttackRate;
    ALfloat ReleaseRate;
    ALfloat GainCtrl;
} ALcompressorState;

static ALvoid ALcompressorState_Destruct(ALcompressorState *UNUSED(state))
{
}

static ALboolean ALcompressorState_deviceUpdate(ALcompressorState *state, ALCdevice *device)
{
    const ALfloat attackTime = device->Frequency * 0.2f; /* 200ms Attack */
    const ALfloat releaseTime = device->Frequency * 0.4f; /* 400ms Release */

    state->AttackRate = 1.0f / attackTime;
    state->ReleaseRate = 1.0f / releaseTime;

    return AL_TRUE;
}

static ALvoid ALcompressorState_update(ALcompressorState *state, ALCdevice *Device, const ALeffectslot *Slot)
{
    ALfloat gain;

    state->Enabled = Slot->EffectProps.Compressor.OnOff;

    gain = sqrtf(1.0f / Device->NumChan) * Slot->Gain;
    SetGains(Device, gain, state->Gain);
}

static ALvoid ALcompressorState_process(ALcompressorState *state, ALuint SamplesToDo, const ALfloat *SamplesIn, ALfloat (*SamplesOut)[BUFFERSIZE])
{
    ALuint it, kt;
    ALuint base;

    for(base = 0;base < SamplesToDo;)
    {
        ALfloat temps[64];
        ALuint td = minu(SamplesToDo-base, 64);

        if(state->Enabled)
        {
            ALfloat output, smp, amplitude;
            ALfloat gain = state->GainCtrl;

            for(it = 0;it < td;it++)
            {
                smp = SamplesIn[it+base];

                amplitude = fabsf(smp);
                if(amplitude > gain)
                    gain = minf(gain+state->AttackRate, amplitude);
                else if(amplitude < gain)
                    gain = maxf(gain-state->ReleaseRate, amplitude);
                output = 1.0f / clampf(gain, 0.5f, 2.0f);

                temps[it] = smp * output;
            }

            state->GainCtrl = gain;
        }
        else
        {
            ALfloat output, smp, amplitude;
            ALfloat gain = state->GainCtrl;

            for(it = 0;it < td;it++)
            {
                smp = SamplesIn[it+base];

                amplitude = 1.0f;
                if(amplitude > gain)
                    gain = minf(gain+state->AttackRate, amplitude);
                else if(amplitude < gain)
                    gain = maxf(gain-state->ReleaseRate, amplitude);
                output = 1.0f / clampf(gain, 0.5f, 2.0f);

                temps[it] = smp * output;
            }

            state->GainCtrl = gain;
        }


        for(kt = 0;kt < MaxChannels;kt++)
        {
            ALfloat gain = state->Gain[kt];
            if(!(gain > GAIN_SILENCE_THRESHOLD))
                continue;

            for(it = 0;it < td;it++)
                SamplesOut[kt][base+it] += gain * temps[it];
        }

        base += td;
    }
}

DECLARE_DEFAULT_ALLOCATORS(ALcompressorState)

DEFINE_ALEFFECTSTATE_VTABLE(ALcompressorState);


typedef struct ALcompressorStateFactory {
    DERIVE_FROM_TYPE(ALeffectStateFactory);
} ALcompressorStateFactory;

static ALeffectState *ALcompressorStateFactory_create(ALcompressorStateFactory *UNUSED(factory))
{
    ALcompressorState *state;

	state = (ALcompressorState*) ALcompressorState_New(sizeof(*state));
    if(!state) return NULL;
    SET_VTABLE2(ALcompressorState, ALeffectState, state);

    state->Enabled = AL_TRUE;
    state->AttackRate = 0.0f;
    state->ReleaseRate = 0.0f;
    state->GainCtrl = 1.0f;

    return STATIC_CAST(ALeffectState, state);
}

DEFINE_ALEFFECTSTATEFACTORY_VTABLE(ALcompressorStateFactory);

ALeffectStateFactory *ALcompressorStateFactory_getFactory(void)
{
    static ALcompressorStateFactory CompressorFactory = { { GET_VTABLE2(ALcompressorStateFactory, ALeffectStateFactory) } };

    return STATIC_CAST(ALeffectStateFactory, &CompressorFactory);
}


void ALcompressor_setParami(ALeffect *effect, ALCcontext *context, ALenum param, ALint val)
{
    ALeffectProps *props = &effect->Props;
    switch(param)
    {
        case AL_COMPRESSOR_ONOFF:
            if(!(val >= AL_COMPRESSOR_MIN_ONOFF && val <= AL_COMPRESSOR_MAX_ONOFF))
                SET_ERROR_AND_RETURN(context, AL_INVALID_VALUE);
            props->Compressor.OnOff = val;
            break;

    default:
        SET_ERROR_AND_RETURN(context, AL_INVALID_ENUM);
    }
}
void ALcompressor_setParamiv(ALeffect *effect, ALCcontext *context, ALenum param, const ALint *vals)
{
    ALcompressor_setParami(effect, context, param, vals[0]);
}
void ALcompressor_setParamf(ALeffect *UNUSED(effect), ALCcontext *context, ALenum UNUSED(param), ALfloat UNUSED(val))
{ SET_ERROR_AND_RETURN(context, AL_INVALID_ENUM); }
void ALcompressor_setParamfv(ALeffect *effect, ALCcontext *context, ALenum param, const ALfloat *vals)
{
    ALcompressor_setParamf(effect, context, param, vals[0]);
}

void ALcompressor_getParami(const ALeffect *effect, ALCcontext *context, ALenum param, ALint *val)
{ 
    const ALeffectProps *props = &effect->Props;
    switch(param)
    {
        case AL_COMPRESSOR_ONOFF:
            *val = props->Compressor.OnOff;
            break;
        default:
            SET_ERROR_AND_RETURN(context, AL_INVALID_ENUM);
    }
}
void ALcompressor_getParamiv(const ALeffect *effect, ALCcontext *context, ALenum param, ALint *vals)
{
    ALcompressor_getParami(effect, context, param, vals);
}
void ALcompressor_getParamf(const ALeffect *UNUSED(effect), ALCcontext *context, ALenum UNUSED(param), ALfloat *UNUSED(val))
{ SET_ERROR_AND_RETURN(context, AL_INVALID_ENUM); }
void ALcompressor_getParamfv(const ALeffect *effect, ALCcontext *context, ALenum param, ALfloat *vals)
{
    ALcompressor_getParamf(effect, context, param, vals);
}

DEFINE_ALEFFECT_VTABLE(ALcompressor);
