#include <pokeagb/pokeagb.h>
#include "../battle_data/pkmn_bank.h"
#include "../battle_data/pkmn_bank_stats.h"
#include "../battle_data/battle_state.h"

extern void dprintf(const char * str, ...);
extern bool enqueue_message(u16 move, u8 bank, enum battle_string_ids id, u16 effect);

/* Perish song */
u8 perish_song_on_residual(u8 user, u8 src, u16 move, struct anonymous_callback* acb)
{
    if (user != src) return true;
    for (u8 i = 0; i < BANK_MAX; i++) {
        if (HAS_VOLATILE(i, VOLATILE_PERISH_SONG)) {
            enqueue_message(0, i, STRING_COUNT_FELL, 3 - p_bank[i]->b_data.perish_song_counter);
            p_bank[i]->b_data.perish_song_counter += 1;
            if (p_bank[i]->b_data.perish_song_counter > 3) {
                B_CURRENT_HP(i) = 0;
                B_IS_FAINTED(i) = true;
            }
        }
    }
    return true;
}

u8 perish_song_before_move(u8 user, u8 src, u16 move, struct anonymous_callback* acb)
{
    return true;
}

u8 perish_song_on_effect(u8 user, u8 src, u16 move, struct anonymous_callback* acb)
{
    if (user != src) return true;
    /* Perish song marks each target it hits with a volatile, and sets the perish song counter to 0.
    Already affected targets aren't reapplied perish song. */
    u8 target = TARGET_OF(user);
    if (HAS_VOLATILE(target, VOLATILE_PERISH_SONG)) return true;

    u8 id = get_callback_src((u32)perish_song_before_move, src);
    CB_MASTER[id].data_ptr = true;
    ADD_VOLATILE(target, VOLATILE_PERISH_SONG);
    p_bank[target]->b_data.perish_song_counter = 0;
    return true;
}

void perish_song_on_after_move(u8 user, u8 src, u16 move, struct anonymous_callback* acb)
{
    if (user != src) return;
    u8 id = get_callback_src((u32)perish_song_before_move, src);
    if (CB_MASTER[id].data_ptr == true) {
        enqueue_message(0, user, STRING_PERISH_START, 0);
        add_callback(CB_ON_RESIDUAL, 0, 3, user, (u32)perish_song_on_residual);
    } else {
        enqueue_message(0, user, STRING_FAILED, 0);
    }
}


/* Destiny Bond */