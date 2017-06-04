#ifndef MOVE_TABLE_DATA_H_
#define MOVE_TABLE_DATA_H_

#include <pokeagb/pokeagb.h>
#include "../battle_data/pkmn_bank.h"
#include "../battle_state.h"


#define FLAG_CHARGEUP (1 << 1)
#define FLAG_RECHARGE (1 << 2)
#define FLAG_CONTACT (1 << 3)
#define FLAG_DEFROST (1 << 4)
#define FLAG_GRAVITY_DISABLED (1 << 5)
#define FLAG_HEAL_BLOCK_DISABLED (1 << 6)
#define FLAG_MIRRORABLE (1 << 7)
#define FLAG_POWDER (1 << 8)
#define FLAG_PROTECTABLE (1 << 9)
#define FLAG_IGNORE_SUBSTITUTE (1 << 10)
#define FLAG_IRON_FIST (1 << 11)
#define FLAG_REFLECTABLE (1 << 12)
#define FLAG_SNATCH (1 << 13)
#define FLAG_SOUND_BASED (1 << 14)
#define FLAG_STRONG_JAW (1 << 15)
#define FLAG_BULLET_PROOF (1 << 16)
#define FLAG_ONSELF (1 << 17)
#define FLAG_TARGET (1 << 18)
#define FLAG_HITS_ALL (1 << 19)
#define FLAG_TRIAGE_AFFECTED (1 << 20)
#define FLAGS_UNUSED (1 << 21)

// return base power given bank id and default power. bp_cb(user_bank, bp)
typedef u8 (*BasePowerCallback)(u8, u8);

// activate some p_bank flags for a Pokemon in a certain bank and/or target bank. bt_cb(user_bank, target_bank)
typedef void (*BeforeTurnCallback)(u8);

// Executed before duration for something is applied, to modify duration. dur_cb(user bank, target bank, effect)
typedef u8 (*DurationCallback)(u8, u8, u8);

// executed before damage calculation, and skips dmg_calc. dmg_cb(user_bank, target_bank)
typedef u16 (*DamageCallback)(u8, u8);

// executed right before using a move. bm_cb(user_bank)
typedef void (*BeforeMoveCallback)(u8);

enum MoveTypes {
    MTYPE_NORMAL = 0,
    MTYPE_FIGHTING,
    MTYPE_FLYING,
    MTYPE_POISON,
    MTYPE_GROUND,
    MTYPE_ROCK,
    MTYPE_BUG,
    MTYPE_GHOST,
    MTYPE_STEEL,
    MTYPE_EGG,
    MTYPE_FIRE,
    MTYPE_WATER,
    MTYPE_GRASS,
    MTYPE_ELECTRIC,
    MTYPE_PSYCHIC,
    MTYPE_ICE,
    MTYPE_DRAGON,
    MTYPE_DARK,
    MTYPE_FAIRY,
};

enum MoveCategory {
    MOVE_PHYSICAL,
    MOVE_SPECIAL,
    MOVE_STATUS,
};

enum PokemonStat {
    STAT_ATTACK,
    STAT_DEFENSE,
    STAT_SPEED,
    STAT_SPECIAL_ATTACK,
    STAT_SPECIAL_DEFENSE,
    STAT_NONE,
};

/*

List of flags and their descriptions:

authentic: Ignores a target's substitute.
bite: Power is multiplied by 1.5 when used by a Pokemon with the Ability Strong Jaw.
bullet: Has no effect on Pokemon with the Ability Bulletproof.
charge: The user is unable to make a move between turns.
contact: Makes contact.
defrost: Thaws the user if executed successfully while the user is frozen.
gravity: Prevented from being executed or selected during Gravity's effect.
heal: Prevented from being executed or selected during Heal Block's effect.
mirror: Can be copied by Mirror Move.
powder: Has no effect on Grass-type Pokemon, Pokemon with the Ability Overcoat, and Pokemon holding Safety Goggles.
protect: Blocked by Detect, Protect, Spiky Shield, and if not a Status move, King's Shield.
pulse: Power is multiplied by 1.5 when used by a Pokemon with the Ability Mega Launcher.
punch: Power is multiplied by 1.2 when used by a Pokemon with the Ability Iron Fist.
recharge: If this move is successful, the user must recharge on the following turn and cannot make a move.
reflectable: Bounced back to the original user by Magic Coat or the Ability Magic Bounce.
snatch: Can be stolen from the original user and instead used by another Pokemon using Snatch.
sound: Has no effect on Pokemon with the Ability Soundproof.
    
*/	

struct move_callbacks {
    BasePowerCallback bp_cb;
    BeforeTurnCallback bt_cb;
    DurationCallback dur_cb;
    DamageCallback dmg_cb;
    BeforeMoveCallback bm_cb;
    
};

struct move_procs {
    u8 chance_self;
    u8 chance_target;
    enum PokemonStat stat_self;
    enum PokemonStat stat_target;
    s8 amount_self;
    s8 amount_target;
    u8 multihit_lowest;
    u8 multihit_highest;
};


struct move_data {
    pchar name[30];
    u8 accuracy;
    u8 base_power;
    pchar* description;
    u8 pp;
    s8 priority;
    u8 crit_ratio;
    enum MoveCategory category;
    enum MoveTypes type;
    u32* m_flags;
    struct move_procs* procs;
    struct move_callbacks* move_cb;
};

extern struct move_data move_t[];
extern u32 m_flags[672];



#endif /* MOVE_TABLE_DATA_H_ */	
