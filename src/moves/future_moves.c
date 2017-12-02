#include <pokeagb/pokeagb.h>
#include "../battle_data/pkmn_bank.h"
#include "../battle_data/pkmn_bank_stats.h"
#include "../battle_data/battle_state.h"

extern void dprintf(const char * str, ...);
extern bool enqueue_message(u16 move, u8 bank, enum battle_string_ids id, u16 effect);
extern void do_damage(u8 bank_index, u16 dmg);
extern u16 get_damage(u8 attacker, u8 defender, u16 move);
extern void do_heal(u8 bank_index, u8 percent_heal);
extern void set_status(u8 bank, enum Effect status);

//Future Sight
u16 future_sight_on_residual(u8 user, u8 src, u16 move, struct anonymous_callback* acb)
{
    if (acb->duration == 0) {
        // do damage
        B_MOVE_TYPE(user, 0) = MTYPE_NONE;
        B_MOVE_POWER(user) = 120;
        B_MOVE_ACCURACY(user) = 100;
        B_MOVE_CATEGORY(user) = MOVE_SPECIAL;
        B_MOVE_CAN_CRIT(user) = true;
        u16 dmg = get_damage(user, (u8)acb->data_ptr, CURRENT_MOVE(user));
        do_damage((u8)acb->data_ptr, MAX(1, dmg));
        acb->in_use = false;
        enqueue_message(MOVE_FUTURE_SIGHT, (u8)acb->data_ptr, STRING_TOOK_ATTACK, 0);
    }
    return true;
}

u8 future_sight_on_trhit(u8 user, u8 src, u16 move, struct anonymous_callback* acb)
{
    if (user != src) return true;
    if (callback_exists((u32)future_sight_on_residual)) return false;
    u8 id = add_callback(CB_ON_RESIDUAL, 0, 3, user, (u32)future_sight_on_residual);
    CB_MASTER[id].data_ptr = TARGET_OF(src);
    enqueue_message(MOVE_FUTURE_SIGHT, user, STRING_FUTURE_FORESAW, 0);
    return 3;
}

// Wish
u16 wish_on_residual(u8 user, u8 src, u16 move, struct anonymous_callback* acb)
{
    if (user != src) return true;
    if (acb->duration == 0) {
        do_heal(src, 50);
        enqueue_message(0, user, STRING_WISH_TRUE, 0);
        acb->in_use = false;
    }
    return true;
}

u8 wish_on_effect(u8 user, u8 src, u16 move, struct anonymous_callback* acb)
{
    if (user != src) return true;
    if (!callback_exists((u32)wish_on_residual)) {
        add_callback(CB_ON_RESIDUAL, 0, 1, src, (u32)wish_on_residual);
        return true;
    }
    return false;
}


// Yawn
u16 yawn_on_residual(u8 user, u8 src, u16 move, struct anonymous_callback* acb)
{
    if (user != (FOE_BANK(src))) return true;
    if (acb->duration == 0) {
        set_status(user, AILMENT_SLEEP);
        acb->in_use = false;
    }
    return true;
}

u8 yawn_on_trhit(u8 user, u8 src, u16 move, struct anonymous_callback* acb)
{
    if (user!= src) return true;
    if (B_STATUS(user))
        return false;
    return true;
}

u8 yawn_on_effect(u8 user, u8 src, u16 move, struct anonymous_callback* acb)
{
    dprintf("%d is sid of STRINGS_TABLE_MAX",STRINGS_TABLE_MAX);
    if (user != src) return true;
    if (B_STATUS(user)) return false;
    add_callback(CB_ON_RESIDUAL, 0, 1, src, (u32)yawn_on_residual);
    enqueue_message(0, user, STRING_GREW_DROWSY, 0);
    return true;
}


// Charge, Doom Desire
void charge_on_base_power_move(u8 user, u8 src, u16 move, struct anonymous_callback* acb)
{
    if (user != src) return;
    dprintf("gonna charge up that move\n");
    if (B_MOVE_HAS_TYPE(user, MTYPE_ELECTRIC))
        B_MOVE_POWER(user) *= 2;
}

u8 charge_on_effect(u8 user, u8 src, u16 move, struct anonymous_callback* acb)
{
    add_callback(CB_ON_RESIDUAL, 1, 0, src, (u32)charge_on_base_power_move);
    return true;
}
