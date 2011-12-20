//
// Copyright 2011 Josh Blum
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <tsbe/buffer.h>
#include "types_impl.h"
#include <string.h> //memcpy

void tsbe__buffer__init(
    tsbe__buffer_t *buff_,
    const tbse__buffer__config_t *config
){
    tsbe_impl__buffer_t *buff = (tsbe_impl__buffer_t *)buff_;
    memcpy(&buff->config, config, sizeof(*config));
    buff->length = config->length;
}

void tsbe__buffer__done(tsbe__buffer_t *buff_, const size_t num_readers){
    tsbe_impl__buffer_t *buff = (tsbe_impl__buffer_t *)buff_;
    const size_t count = AO_fetch_and_add(&buff->count, ((int)num_readers)-1);
    if (count == 0 && buff->config.done_cb != NULL){
        buff->length = buff->config.length; //reset length
        (*buff->config.done_cb)(buff->config.done_data);
    }
}

int tsbe__buffer__get_mode_flags(const tsbe__buffer_t *buff_){
    tsbe_impl__buffer_t *buff = (tsbe_impl__buffer_t *)buff_;
    if (AO_load(&buff->count) == 1) return buff->config.mode_flags;
    return buff->config.mode_flags & ~TBSE__BUFFER__MODE_FLAG_WR;
}

void *tsbe__buffer__get_memory(const tsbe__buffer_t *buff_){
    tsbe_impl__buffer_t *buff = (tsbe_impl__buffer_t *)buff_;
    return buff->config.memory;
}

size_t tsbe__buffer__get_length(const tsbe__buffer_t *buff_){
    tsbe_impl__buffer_t *buff = (tsbe_impl__buffer_t *)buff_;
    return buff->length;
}

void tsbe__buffer__set_length(tsbe__buffer_t *buff_, const size_t length){
    tsbe_impl__buffer_t *buff = (tsbe_impl__buffer_t *)buff_;
    buff->length = length;
}
