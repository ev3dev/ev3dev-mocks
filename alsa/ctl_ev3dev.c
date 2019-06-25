// SPDX-License-Identifier: GPL-2.0-only
// Copyright (C) 2019 David Lechner <david@pybricks.com>

// This is an ALSA ctl plugin that simulates the sound driver found in the
// ev3dev operating system. It provides two volume controls, Beep and PCM.

#include <errno.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include <asoundlib.h>
#include <alsa/control_external.h>

#define EV3DEV_MAX_VOLUME 256
#define EV3DEV_BEEP_ELEMENT_NAME "Beep Playback Volume"
#define EV3DEV_PCM_ELEMENT_NAME "PCM Playback Volume"

typedef struct {
    snd_ctl_ext_t ext;
    int beep_volume;
    int pcm_volume;
} ev3dev_info_t;

/**
 * close the control handle; optional
 */
static void ev3dev_close(snd_ctl_ext_t *ext) {
    ev3dev_info_t *info = ext->private_data;

    assert(info);

    free(info);
}

/**
 * return the total number of elements; required
 */
static int ev3dev_elem_count(snd_ctl_ext_t *ext) {
    return 2;
}

/**
 * return the element id of the given offset (array index); required
 */
static int ev3dev_elem_list(snd_ctl_ext_t *ext, unsigned int offset, snd_ctl_elem_id_t *id) {
    snd_ctl_elem_id_set_interface(id, SND_CTL_ELEM_IFACE_MIXER);

    switch (offset) {
    case 0:
         snd_ctl_elem_id_set_name(id, EV3DEV_PCM_ELEMENT_NAME);
         break;
    case 1:
        snd_ctl_elem_id_set_name(id, EV3DEV_BEEP_ELEMENT_NAME);
        break;
    }

    return 0;
}

/**
 * convert the element id to a search key; required
 */
static snd_ctl_ext_key_t ev3dev_find_elem(snd_ctl_ext_t *ext, const snd_ctl_elem_id_t *id) {
    const char *name;
    unsigned int numid;

    numid = snd_ctl_elem_id_get_numid(id);
    if (numid > 0 && numid <= 2) {
        return numid - 1;
    }

    name = snd_ctl_elem_id_get_name(id);
    if (strcmp(name, EV3DEV_PCM_ELEMENT_NAME) == 0) {
        return 0;
    }
    if (strcmp(name, EV3DEV_BEEP_ELEMENT_NAME) == 0) {
        return 1;
    }

    return SND_CTL_EXT_KEY_NOT_FOUND;
}

/**
 * the destructor of the key; optional
 */
//void (*free_key)(snd_ctl_ext_t *ext, snd_ctl_ext_key_t key);

/**
 * get the attribute of the element; required
 */
static int ev3dev_get_attribute(snd_ctl_ext_t *ext, snd_ctl_ext_key_t key,
                                int *type, unsigned int *acc, unsigned int *count) {

    *type = SND_CTL_ELEM_TYPE_INTEGER;
    *acc = SND_CTL_EXT_ACCESS_READWRITE;
    *count = 1;

    return 0;
}

/**
 * get the element information of integer type
 */
static int ev3dev_get_integer_info(snd_ctl_ext_t *ext, snd_ctl_ext_key_t key,
                                   long *imin, long *imax, long *istep) {

    *imin = 0;
    *imax = EV3DEV_MAX_VOLUME;
    *istep = 1;

    return 0;
}

#if 0
/**
 * get the element information of integer64 type
 */
int (*get_integer64_info)(snd_ctl_ext_t *ext, snd_ctl_ext_key_t key,
                int64_t *imin, int64_t *imax, int64_t *istep);
/**
 * get the element information of enumerated type
 */
int (*get_enumerated_info)(snd_ctl_ext_t *ext, snd_ctl_ext_key_t key, unsigned int *items);
/**
 * get the name of the enumerated item
 */
int (*get_enumerated_name)(snd_ctl_ext_t *ext, snd_ctl_ext_key_t key, unsigned int item,
                char *name, size_t name_max_len);
#endif

/**
 * read the current values of integer type
 */
static int ev3dev_read_integer(snd_ctl_ext_t *ext, snd_ctl_ext_key_t key, long *value) {
    ev3dev_info_t *info = ext->private_data;

    assert(info);

    switch (key) {
    case 0:
        value[0] = info->pcm_volume;
        break;
    case 1:
        value[0] = info->beep_volume;
        break;
    default:
        return -EINVAL;
    }

    return 0;
}

#if 0
/**
 * read the current values of integer64 type
 */
int (*read_integer64)(snd_ctl_ext_t *ext, snd_ctl_ext_key_t key, int64_t *value);
/**
 * read the current values of enumerated type
 */
int (*read_enumerated)(snd_ctl_ext_t *ext, snd_ctl_ext_key_t key, unsigned int *items);
/**
 * read the current values of bytes type
 */
int (*read_bytes)(snd_ctl_ext_t *ext, snd_ctl_ext_key_t key, unsigned char *data,
            size_t max_bytes);
/**
 * read the current values of iec958 type
 */
int (*read_iec958)(snd_ctl_ext_t *ext, snd_ctl_ext_key_t key, snd_aes_iec958_t *iec958);
#endif

/**
 * update the current values of integer type with the given values
 */
static int ev3dev_write_integer(snd_ctl_ext_t *ext, snd_ctl_ext_key_t key, long *value) {
    ev3dev_info_t *info = ext->private_data;

    assert(info);

    switch (key) {
    case 0:
        info->pcm_volume = value[0];
        break;
    case 1:
        info->beep_volume = value[0];
        break;
    default:
        return -EINVAL;
    }

    return 0;
}

#if 0
/**
 * update the current values of integer64 type with the given values
 */
int (*write_integer64)(snd_ctl_ext_t *ext, snd_ctl_ext_key_t key, int64_t *value);
/**
 * update the current values of enumerated type with the given values
 */
int (*write_enumerated)(snd_ctl_ext_t *ext, snd_ctl_ext_key_t key, unsigned int *items);
/**
 * update the current values of bytes type with the given values
 */
int (*write_bytes)(snd_ctl_ext_t *ext, snd_ctl_ext_key_t key, unsigned char *data,
            size_t max_bytes);
/**
 * update the current values of iec958 type with the given values
 */
int (*write_iec958)(snd_ctl_ext_t *ext, snd_ctl_ext_key_t key, snd_aes_iec958_t *iec958);
/**
 * subscribe/unsubscribe the event notification; optional
 */
void (*subscribe_events)(snd_ctl_ext_t *ext, int subscribe);
/**
 * read a pending notification event; optional
 */
int (*read_event)(snd_ctl_ext_t *ext, snd_ctl_elem_id_t *id, unsigned int *event_mask);
/**
 * return the number of poll descriptors; optional
 */
int (*poll_descriptors_count)(snd_ctl_ext_t *ext);
/**
 * fill the poll descriptors; optional
 */
int (*poll_descriptors)(snd_ctl_ext_t *ext, struct pollfd *pfds, unsigned int space);
/**
 * mangle the revents of poll descriptors
 */
int (*poll_revents)(snd_ctl_ext_t *ext, struct pollfd *pfds, unsigned int nfds, unsigned short *revents);
#endif

static const snd_ctl_ext_callback_t ev3dev_callback = {
    .close              = ev3dev_close,
    .elem_count         = ev3dev_elem_count,
    .elem_list          = ev3dev_elem_list,
    .find_elem          = ev3dev_find_elem,
    .get_attribute      = ev3dev_get_attribute,
    .get_integer_info   = ev3dev_get_integer_info,
    .read_integer       = ev3dev_read_integer,
    .write_integer      = ev3dev_write_integer,
};

SND_CTL_PLUGIN_DEFINE_FUNC(ev3dev) {
    snd_config_iterator_t i, next;
    ev3dev_info_t *info;
    int err;

    snd_config_for_each(i, next, conf) {
        snd_config_t *n = snd_config_iterator_entry(i);
        const char *id;
        if (snd_config_get_id(n, &id) < 0)
            continue;
        if (strcmp(id, "comment") == 0 || strcmp(id, "type") == 0)
            continue;
        // TODO: can add additional config options here if needed
        SNDERR("Unknown field %s", id);
        return -EINVAL;
    }

    info = calloc(1, sizeof(*info));
    if (info == NULL) {
        return -ENOMEM;
    }

    info->ext.version = SND_CTL_EXT_VERSION;
    info->ext.card_idx = 0;
    strcpy(info->ext.id, "ev3dev");
    strcpy(info->ext.driver, "ev3dev mock");
    strcpy(info->ext.name, "LEGO MINDSTORMS EV3 Speaker"); // Card
    strcpy(info->ext.longname, "LEGO MINDSTORMS EV3 Speaker connected to nothing");
    strcpy(info->ext.mixername, ""); // Chip
    info->ext.poll_fd = -1;
    info->ext.callback = &ev3dev_callback;
    info->ext.private_data = info;

    info->beep_volume = EV3DEV_MAX_VOLUME / 5; // 20%
    info->pcm_volume = EV3DEV_MAX_VOLUME; // 100%

    err = snd_ctl_ext_create(&info->ext, name, mode);
    if (err < 0) {
        free(info);
        return err;
    }
    *handlep = info->ext.handle;
    return 0;
}
SND_CTL_PLUGIN_SYMBOL(ev3dev);
