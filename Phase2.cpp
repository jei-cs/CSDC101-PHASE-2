#include <iostream>
#include <string>
#include <iomanip>
#include <limits>
#include <cmath>
using namespace std;

// PLAYER ARRAY
// Stat index mapping: 
// [0] = HP_max
// [1] = HP_current
// [2] = ATK
// [3] = END
// [4] = EP_max
// [5] = EP_current
// [6] = EP_regen
// [7] = Self Discovery
const int player = 1;
const int stats = 8;
const int skillCount = 9;
double player_stats[player][stats] = {
	{50, 50, 10, 15, 5, 5, 0, 0} 
};

string player_name[player] = {"Keigan"};
int player_training_history[player][stats] = {{0,0,0,0,0,0,0}};
int player_skills[player][skillCount] = {{0,0,0,0,0,0,0,0}};

// STAT INDICES
const int hp_max = 0;
const int hp_current = 1;
const int atk = 2;
const int end_stat = 3;
const int ep_max = 4;
const int ep_current = 5;
const int ep_regen = 6;
const int self_discovery = 7;

// SKILL ARRAYS 
// Skill Index Reference
// 0 = Eight Gates - Gate of Opening (G1)
// 1 = Chain Handling
// 2 = Kei-ga-n Barrage
// 3 = Front Lotus (Omote Renge)
// 4 = Gate of Opening - Mastery
// 5 = Gate of Healing (G2)
// 6 = Chain Barrage
// 7 = Reverse Lotus (Ura Renge)
// 8 = Art of Resistance

const int SKILLS = 9;
string skill_name[SKILLS] = {
	"Eight Gates - Gate of Opening (G1)",
	"Chain Handling",
	"Kei-ga-n Barrage",
	"Front Lotus (Omote Renge)",
	"Gate of Opening - Mastery",
	"Gate of Healing (G2)",
	"Chain Barrage",
	"Reverse Lotus (Ura Renge)",
	"Art of Resistance"
};

string skill_type[SKILLS] = {
	"Stance / Toggle",
	"Passive",
	"Combo Attack",
	"Finisher",
	"Passive",
	"Stance / Toggle",
	"Combo Attack",
	"Finisher",
	"Buff / Self-Defense"
};

string skill_description[SKILLS] = {
	"Keigan unlocks the first of the Eight Gates. Boosts ATK x 1.8, costs -35 EP on cast, -7 per turn.",
	"A fast 3-hit combo using fists and leg strikes. Each hit: ATK x 0.45, ignores 5% of enemy END. Core poke attack. CD: 2 turns.",
	"Allows use of chain-based moves and throws. Enables the Lotus series.",
	"A chain-assisted aerial throw. Each hit: ATK x 0.95, ignores 30% of END. Requires Chains + Gate ON + next turn after Barrage.",
	"Reduces Gate EP cost to -30 cast, -5 per turn. Crash penalty softens to ÷1.4 for ATK/END.",
	"Increases ATK x 2.2, with -45 EP cast, -9 per turn.",
	"Enhanced 3-hit chain combo. Each hit: ATK x 0.5, ignores 10% END. Stronger variant of Kei-ga-n Barrage.",
	"Devastating 4-hit finisher. Each hit: ATK x 1.25, ignores 35% END. Adds +1 hit if G2 active. Requires Gate ON and Front Lotus used once. CD: 5.",
	"Bolster yourself with defensive energy. Boosts END x 1.5 for 3 turns. Prerequisite: HP must be at least 75% of max. CD: 3 turns."
};

long skill_cost[SKILLS] = {0, 0, 12, 13, 0, 0, 14, 25, 50};

int skill_cooldowns[SKILLS] = {0}; 

void display_final_stats(int playerIndex) {
	cout << "========================================" << endl;
	cout << "   " << player_name[playerIndex] << "'s Final Stats" << endl;
	cout << "----------------------------------------" << endl;
	cout << "HP Max: " << (long)player_stats[playerIndex][hp_max] << endl;
	cout << "ATK: " << (long)player_stats[playerIndex][atk] << endl;
	cout << "END: " << (long)player_stats[playerIndex][end_stat] << endl;
	cout << "EP Max: " << (long)player_stats[playerIndex][ep_max] << endl;
	cout << "EP Regen: " << player_stats[playerIndex][ep_regen] << endl;
	cout << "Self Discovery Level: " << (int)player_stats[playerIndex][self_discovery] << endl;
	cout << "========================================" << endl;
}

bool gate_active_G1 = false;
bool gate_active_G2 = false;
bool gate_crashed = false;
bool frontLotusUsed = false;
bool reverseLotusUsed = false;
bool artOfResistanceActive = false;
int artOfResistanceTurns = 0;
double saved_end_resistance = 0;

// GATE VARIABLES
double active_atk_multiplier = 1.0;
int gate_cast_cost_G1 = 35;
int gate_per_turn_G1 = 7;
int gate_cast_cost_G2 = 45;
int gate_per_turn_G2 = 9;

// GATE CRASH VARIABLES
double saved_atk[player] = {0};
double saved_end[player] = {0};
bool gate_crash_active[player] = {false};
int gate_crash_turns[player] = {0};

bool gate_end_debuff_active = false; 
int gate_end_debuff_required_EP = 0; 
double gate_crash_divisor_default = 1.5; 
bool gate_just_activated_G1[player] = {false};
bool gate_just_activated_G2[player] = {false};

// SKILL LIST  AND FUNCTIONS
void displayskills_training(int playerIndex) {
	cout << "========================================" << endl;
	cout << "   " << player_name[playerIndex] << "'s Skill List (Training)" << endl;
	cout << "----------------------------------------" << endl;
	int maxLearned = (int)player_stats[playerIndex][self_discovery];
	for (int i = 0; i < maxLearned; i++) {
		if (player_skills[playerIndex][i]) {
			cout << "[" << (i + 1) << "] " << skill_name[i] << "\n     Type: " << skill_type[i] << " | " << "Description: " << skill_description[i] << "\n";
			cout << "     Cast Cost: " << skill_cost[i] << " EP" << "\n";
			cout << "----------------------------------------" << endl;
		}
	}
	cout << "========================================" << endl;
}
	
void displayskills_battle(int playerIndex) {
	cout << "========================================" << endl;
	cout << "   " << player_name[playerIndex] << "'s Skills (Battle)" << endl;
	cout << "----------------------------------------" << endl;
	cout << "1) " << skill_name[2] << "\n     Cost: " << skill_cost[2] << " EP | CD: " << skill_cooldowns[2] << " | " << skill_type[2] << "\n     " << skill_description[2] << "\n";
	cout << "----------------------------------------" << endl;
	cout << "2) " << skill_name[3] << "\n     Cost: " << skill_cost[3] << " EP | CD: " << skill_cooldowns[3] << " | " << skill_type[3] << "\n     " << skill_description[3] << "\n";
	cout << "----------------------------------------" << endl;
	cout << "3) " << skill_name[6] << "\n     Cost: " << skill_cost[6] << " EP | CD: " << skill_cooldowns[6] << " | " << skill_type[6] << "\n     " << skill_description[6] << "\n";
	cout << "----------------------------------------" << endl;
	cout << "4) " << skill_name[7] << "\n     Cost: " << skill_cost[7] << " EP | CD: " << skill_cooldowns[7] << " | " << skill_type[7] << "\n     " << skill_description[7] << "\n";
	cout << "----------------------------------------" << endl;
	cout << "5) " << skill_name[8] << "\n     Cost: " << skill_cost[8] << " EP | CD: " << skill_cooldowns[8] << " | " << skill_type[8] << "\n     " << skill_description[8] << "\n";
	cout << "========================================" << endl;
}

// BOSS ARRAY
// Stat index mapping:
// [0] = HP_max
// [1] = ATK
// [2] = END
const int phase = 3;
const int boss_stats = 3;
double boss[phase][boss_stats] = {
	{2500, 520, 600},   // Phase 1: Base Form
	{1600, 550, 600},   // Phase 2: Kiribachi Form
	{900, 600, 660}    // Phase 3: Enraged Form
};

const int base_form = 0;
const int kiribachi = 1;
const int enraged = 2;
const int boss_hp = 0;
const int boss_atk = 1;
const int boss_end = 2;

// FORMULA FUNCTIONS
long ApplyStatGain(int statType, int n) {
    switch (statType) {
        case hp_max: return 21 + 4 * (n - 1);
        case atk: return 11 + 2 * (n - 1);
        case end_stat: return 15 + 3 * (n - 1);
        case ep_max: return 15 + 3 * (n - 1);
        case ep_regen: return (double)(2.5 + 0.5 * (n - 1)); 
        default: return 0;
    }
}

void selfdiscoverygain(int playerIndex){
		player_stats[playerIndex][self_discovery] += 1;
		player_training_history[playerIndex][self_discovery] += 1;

		int SD_level = (int)player_stats[playerIndex][self_discovery];
		int skill = SD_level - 1;
        
		if(player_skills[playerIndex][skill] == 0){
			player_skills[playerIndex][skill] = 1;
			cout << "You have increased your Self Discovery to level " << SD_level << " and learned a new skill: " << skill_name[skill] << "!" << endl;
			cout << "Skill Description: " << skill_description[skill] << endl;
			cout << " " << endl;
		}
	

}

// BATTLE LOOP FUNCTIONS
void TickCooldownsAndStatuses(int playerIndex) {
		for (int i = 0; i < SKILLS; i++) {
			if (skill_cooldowns[i] > 0) skill_cooldowns[i]--;
		}

		if (gate_crash_active[playerIndex]) {
			gate_crash_turns[playerIndex]--;
			if (gate_crash_turns[playerIndex] <= 0) {
				player_stats[playerIndex][atk] = saved_atk[playerIndex];
				player_stats[playerIndex][end_stat] = saved_end[playerIndex];
				gate_crash_active[playerIndex] = false;
				cout << "Gate crash penalty has expired; ATK and END restored." << endl;
			}
		}

		if (gate_end_debuff_active && player_stats[playerIndex][ep_current] >= gate_end_debuff_required_EP) {
			gate_end_debuff_active = false;
			gate_end_debuff_required_EP = 0;
			cout << "You have enough EP; gate end debuff cleared. You may activate gates again." << endl;
		}

		if (artOfResistanceActive) {
			artOfResistanceTurns--;
			if (artOfResistanceTurns <= 0) {
				player_stats[playerIndex][end_stat] = saved_end_resistance;
				artOfResistanceActive = false;
				cout << "Art of Resistance buff has expired; END restored." << endl;
			}
		}
	}
	
void BeginTurn(int playerIndex) {
	player_stats[playerIndex][ep_current] += player_stats[playerIndex][ep_regen];
	if (player_stats[playerIndex][ep_current] > player_stats[playerIndex][ep_max]) {
		player_stats[playerIndex][ep_current] = player_stats[playerIndex][ep_max];
	}
}

int ApplyDamage(int attackerATK, int defenderEND) {
	int damage = max(1, (int)ceil((attackerATK - defenderEND)/2.0));
	return damage;
}

bool skill_KeiBarrage (int playerIndex, int &bossPhase, int &bossHP) {
	
	if (player_skills[playerIndex][2] == 0) {
		cout << "You haven't learned Kei-ga-n Barrage." << endl;
		return false;
	}

	if (skill_cooldowns[2] > 0) {
		cout << skill_name[2] << " has " << skill_cooldowns[2] << " more turn(s) before you can use it again." << endl;
		return false;
	}

	if (player_stats[playerIndex][ep_current] < skill_cost[2]) {
		cout << "Not enough EP to use " << skill_name[2] << "." << endl;
		return false;
	}

	player_stats[playerIndex][ep_current] -= skill_cost[2];

	int totalDamage = 0;
	for (int hit = 0; hit < 3; hit++) {
		int damage = ApplyDamage((int)(player_stats[playerIndex][atk] * 0.45), (int)(boss[bossPhase][boss_end] * 0.95));
		totalDamage += damage;
	}
	bossHP -= totalDamage;
	skill_cooldowns[2] = 2; 
	cout << "You used Kei-ga-n Barrage and dealt " << totalDamage << " total damage!" << endl;
	return true;
}

bool skill_FrontLotus (int playerIndex, int &bossPhase, int &bossHP) {

	if (player_skills[playerIndex][3] == 0) {
		cout << "You haven't learned Front Lotus." << endl;
		return false;
	}

	if (skill_cooldowns[3] > 0) {
		cout << skill_name[3] << " has " << skill_cooldowns[3] << " more turn(s) before you can use it again." << endl;
		return false;
	}

	if (player_stats[playerIndex][ep_current] < skill_cost[3]) {
		cout << "Not enough EP to use " << skill_name[3] << "." << endl;
		return false;
	}

	
	if (!gate_active_G1) {
		cout << "Front Lotus requires Gate of Opening (G1) to be active." << endl;
		return false;
	}

	player_stats[playerIndex][ep_current] -= skill_cost[3];

	int totalDamage = 0;
	for (int hit = 0; hit < 4; hit++) {
		int damage = ApplyDamage((int)(player_stats[playerIndex][atk] * 0.95), (int)(boss[bossPhase][boss_end] * 0.7));
		totalDamage += damage;
	}
	bossHP -= totalDamage;
	skill_cooldowns[3] = 5; 
	frontLotusUsed = true; 
	cout << "You used Front Lotus and dealt " << totalDamage << " total damage!" << endl;
	return true;
}

bool skill_ChainBarrage (int playerIndex, int &bossPhase, int &bossHP) {

	if (player_skills[playerIndex][6] == 0) {
		cout << "You haven't learned Chain Barrage." << endl;
		return false;
	}

	if (skill_cooldowns[6] > 0) {
		cout << skill_name[6] << " has " << skill_cooldowns[6] << " more turn(s) before you can use it again." << endl;
		return false;
	}

	if (player_stats[playerIndex][ep_current] < skill_cost[6]) {
		cout << "Not enough EP to use " << skill_name[6] << "." << endl;
		return false;
	}

	player_stats[playerIndex][ep_current] -= skill_cost[6];

	int totalDamage = 0;
	for (int hit = 0; hit < 3; hit++) {
		int damage = ApplyDamage((int)(player_stats[playerIndex][atk] * 0.5), (int)(boss[bossPhase][boss_end] * 0.9));
		totalDamage += damage;
	}
	bossHP -= totalDamage;
	skill_cooldowns[6] = 3; 
	cout << "You used Chain Barrage and dealt " << totalDamage << " total damage!" << endl;
	return true;
}

bool skill_ReverseLotus (int playerIndex, int &bossPhase, int &bossHP) {

	if (player_skills[playerIndex][7] == 0) {
		cout << "You haven't learned Reverse Lotus." << endl;
		return false;
	}

	if (skill_cooldowns[7] > 0) {
		cout << skill_name[7] << " has " << skill_cooldowns[7] << " more turn(s) before you can use it again." << endl;
		return false;
	}

	if (player_stats[playerIndex][ep_current] < skill_cost[7]) {
		cout << "Not enough EP to use " << skill_name[7] << "." << endl;
		return false;
	}

	if (!gate_active_G1) {
		cout << "You need to have Gate of Opening active to use Reverse Lotus." << endl;
		return false;
	}

	if (!frontLotusUsed) {
		cout << "You must use Front Lotus before using Reverse Lotus." << endl;
		return false;
	}

	player_stats[playerIndex][ep_current] -= skill_cost[7];
	int totalDamage = 0;
	int hits = 4;
	if (gate_active_G2 && frontLotusUsed) {
		hits += 1; 
	}
	for (int hit = 0; hit < hits; hit++) {
		int damage = ApplyDamage((int)(player_stats[playerIndex][atk] * 1.25), (int)(boss[bossPhase][boss_end] * 0.65));
		totalDamage += damage;
	}
	bossHP -= totalDamage;
	skill_cooldowns[7] = 5; 
	reverseLotusUsed = true; 
	cout << "You used Reverse Lotus and dealt " << totalDamage << " total damage!" << endl;
	return true;
}

bool skill_ArtOfResistance (int playerIndex, int &bossPhase, int &bossHP) {

	if (player_skills[playerIndex][8] == 0) {
		cout << "You haven't learned Art of Resistance." << endl;
		return false;
	}

	if (skill_cooldowns[8] > 0) {
		cout << skill_name[8] << " has " << skill_cooldowns[8] << " more turn(s) before you can use it again." << endl;
		return false;
	}

	if (player_stats[playerIndex][ep_current] < skill_cost[8]) {
		cout << "Not enough EP to use " << skill_name[8] << "." << endl;
		return false;
	}

	double hp_threshold = player_stats[playerIndex][hp_max] * 0.75;
	if (player_stats[playerIndex][hp_current] < hp_threshold) {
		cout << "Your health is too low to use Art of Resistance. You need at least " << (int)hp_threshold << " HP (75% of max)." << endl;
		return false;
	}

	player_stats[playerIndex][ep_current] -= skill_cost[8];

	if (artOfResistanceActive) {
		cout << "Art of Resistance is already active!" << endl;
	}

	saved_end_resistance = player_stats[playerIndex][end_stat];
	player_stats[playerIndex][end_stat] = player_stats[playerIndex][end_stat] * 1.5;
	artOfResistanceActive = true;
	artOfResistanceTurns = 3;
	skill_cooldowns[8] = 3;
	cout << "You activated Art of Resistance! Your END is boosted by 50% for 3 turns!" << endl;
	cout << "END: " << (int)saved_end_resistance << " -> " << (int)player_stats[playerIndex][end_stat] << endl;
	return true;
}

bool UseSkill(int playerIndex, int skillIndex, int &bossPhase, int &bossHP) {

	switch (skillIndex) {

		case 1: // Kei-ga-n Barrage
			{
				return skill_KeiBarrage(playerIndex, bossPhase, bossHP);
			}

			case 2: // Front Lotus (Omote Renge)
			{
				return skill_FrontLotus(playerIndex, bossPhase, bossHP);
			}

			case 3: // Chain Barrage
			{
				return skill_ChainBarrage(playerIndex, bossPhase, bossHP);
			}

		case 4: // Reverse Lotus (Ura Renge)
		{
			return skill_ReverseLotus(playerIndex, bossPhase, bossHP);
		}

		case 5: // Art of Resistance
		{
			return skill_ArtOfResistance(playerIndex, bossPhase, bossHP);
		}

		case 6: // Cancel
		{
			cout << "Skill use cancelled." << endl;
			return false;
		}		default:
			if (skillIndex == 1){
				cout << "Skill used: " << skill_name[skillIndex + 1] << "." << endl;
				return true;
			} 
			if (skillIndex == 2){
				cout << "Skill used: " << skill_name[skillIndex + 1] << "." << endl;
				return true;
			}
			if (skillIndex == 3){
				cout << "Skill used: " << skill_name[skillIndex + 3] << "." << endl;
				return true;
			}
			if (skillIndex == 4){
				cout << "Skill used: " << skill_name[skillIndex + 3] << "." << endl;
				return true;
			}
	}
	return false;
}

void EndGate(int playerIndex, int gateId, bool crashed) {
	double crashDiv = gate_crash_divisor_default;
	if (player_skills[playerIndex][4]) crashDiv = 1.4; 

	if (gateId == 1 && gate_active_G1) {
		if (active_atk_multiplier != 1.0) player_stats[playerIndex][atk] = player_stats[playerIndex][atk] / active_atk_multiplier;
		active_atk_multiplier = 1.0;
		gate_active_G1 = false;
	} else if (gateId == 2 && gate_active_G2) {
		if (active_atk_multiplier != 1.0) player_stats[playerIndex][atk] = player_stats[playerIndex][atk] / active_atk_multiplier;
		active_atk_multiplier = 1.0;
		gate_active_G2 = false;
	}

	if (crashed) {
		saved_atk[playerIndex] = player_stats[playerIndex][atk];
		saved_end[playerIndex] = player_stats[playerIndex][end_stat];
		player_stats[playerIndex][atk] = player_stats[playerIndex][atk] / crashDiv;
		player_stats[playerIndex][end_stat] = player_stats[playerIndex][end_stat] / crashDiv;
		gate_crash_active[playerIndex] = true;
		gate_crash_turns[playerIndex] = 2;

		gate_end_debuff_active = true;
		gate_end_debuff_required_EP = (int)ceil(1.5 * (double)gate_cast_cost_G1);

		cout << "Gate crashed! ATK and END divided by " << crashDiv << " for 1 turn." << endl;
		cout << "Gate activation blocked until EP >= " << gate_end_debuff_required_EP << "." << endl;
	} else {
		cout << "Gate closed." << endl;
	}

	if (gateId == 1) gate_just_activated_G1[playerIndex] = false;
	if (gateId == 2) gate_just_activated_G2[playerIndex] = false;
}

int ToggleGates(int playerIndex) {
	cout << "========================================" << endl;
	cout << " Toggle Gates Menu" << endl;
	cout << "----------------------------------------" << endl;
	cout << " 1) Toggle Gate of Opening (G1)" << endl;
	cout << " 2) Toggle Gate of Healing (G2)" << endl;
	cout << " 3) Back" << endl;
	cout << "----------------------------------------" << endl;
	cout << "Enter choice: ";
	int ch;
	cin >> ch;
	if (cin.fail()) {
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "Invalid input." << endl;
		return -1;
	}

	if (ch == 1) {
		if (!player_skills[playerIndex][0]) {
			cout << "You haven't learned Gate of Opening (G1)." << endl;
			return -1;
		}

		if (!gate_active_G1 && gate_end_debuff_active && player_stats[playerIndex][ep_current] < gate_end_debuff_required_EP) {
			cout << "Gate activation blocked due to recent gate crash. EP >= " << gate_end_debuff_required_EP << " required." << endl;
			return -1;
		}

		if (gate_active_G1) {
			char conf;
			cout << "Confirm deactivation of Gate of Opening (G1)? (y/n): ";
			cin >> conf;
			if (cin.fail()) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); cout << "Invalid input. Cancelled." << endl; return -1; }
			if (conf != 'y' && conf != 'Y') { cout << "Gate deactivation cancelled." << endl; return -1; }
			EndGate(playerIndex, 1, false);
			return 0; 
		} else {
			int castCost = gate_cast_cost_G1;
			int perTurn = gate_per_turn_G1;
			if (player_skills[playerIndex][4]) {
				castCost = 30;
				perTurn = 5;
			}
			if (player_stats[playerIndex][ep_current] < castCost) {
				cout << "Not enough EP to activate Gate of Opening (cost: " << castCost << ")." << endl;
				return -1;
			}
			char conf;
			cout << "Confirm activation of Gate of Opening (G1) for " << castCost << " EP? (y/n): ";
			cin >> conf;
			if (cin.fail()) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); cout << "Invalid input. Cancelled." << endl; return -1; }
			if (conf != 'y' && conf != 'Y') { cout << "Gate activation cancelled." << endl; return -1; }
			player_stats[playerIndex][ep_current] -= castCost;

			if (active_atk_multiplier != 1.0) {
				player_stats[playerIndex][atk] = player_stats[playerIndex][atk] / active_atk_multiplier;
			}
			active_atk_multiplier = 1.8;
			player_stats[playerIndex][atk] = player_stats[playerIndex][atk] * active_atk_multiplier;
			gate_active_G1 = true;
			gate_just_activated_G1[playerIndex] = true;
			gate_cast_cost_G1 = castCost;
			gate_per_turn_G1 = perTurn;
			cout << "Gate of Opening (G1) activated. ATK x" << active_atk_multiplier << "." << endl;
			return 0; 
		}
	} else if (ch == 2) {
		if (!player_skills[playerIndex][5]) {
			cout << "You haven't learned Gate of Healing (G2)." << endl;
			return -1;
		}

		if (!gate_active_G2 && gate_end_debuff_active && player_stats[playerIndex][ep_current] < gate_end_debuff_required_EP) {
			cout << "Gate activation blocked due to recent gate crash. EP >= " << gate_end_debuff_required_EP << " required." << endl;
			return -1;
		}

		if (gate_active_G2) {
			char conf;
			cout << "Confirm deactivation of Gate of Healing (G2)? (y/n): ";
			cin >> conf;
			if (cin.fail()) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); cout << "Invalid input. Cancelled." << endl; return -1; }
			if (conf != 'y' && conf != 'Y') { cout << "Gate deactivation cancelled." << endl; return -1; }
			EndGate(playerIndex, 2, false);
			return 0;
		} else {
			int castCost = gate_cast_cost_G2;
			int perTurn = gate_per_turn_G2;

			if (!gate_active_G1) {
				cout << "Gate of Healing (G2) requires Gate of Opening (G1) to be active." << endl;
				return -1;
			}
	
			if (gate_active_G1) {
				castCost = 10;
				cout << "Gate of Opening is active: Gate of Healing activation costs only 10 EP." << endl;
			}

			if (player_stats[playerIndex][ep_current] < castCost) {
				cout << "Not enough EP to activate Gate of Healing (cost: " << castCost << ")." << endl;
				return -1;
			}
			char conf2;
			cout << "Confirm activation of Gate of Healing (G2) for " << castCost << " EP? (y/n): ";
			cin >> conf2;
			if (cin.fail()) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); cout << "Invalid input. Cancelled." << endl; return -1; }
			if (conf2 != 'y' && conf2 != 'Y') { cout << "Gate activation cancelled." << endl; return -1; }
			player_stats[playerIndex][ep_current] -= castCost;
			if (active_atk_multiplier != 1.0) {
				player_stats[playerIndex][atk] = player_stats[playerIndex][atk] / active_atk_multiplier;
			}
			active_atk_multiplier = 2.2;
			player_stats[playerIndex][atk] = player_stats[playerIndex][atk] * active_atk_multiplier;
			gate_active_G2 = true;
			gate_just_activated_G2[playerIndex] = true;
			gate_cast_cost_G2 = castCost;
			gate_per_turn_G2 = perTurn;
			cout << "Gate of Healing (G2) activated. ATK x" << active_atk_multiplier << "." << endl;
			return 0;
		}
	}
	cout << "" << endl;
	return -1;
}

bool PlayerActionMenu(int playerIndex, int &bossPhase, int &bossHP) {
	cout << "========================================" << endl;
	cout << "   " << player_name[playerIndex] << " - Battle HUD" << endl;
	cout << "----------------------------------------" << endl;
	cout << "HP: " << (long)player_stats[playerIndex][hp_current] << "/" << (long)player_stats[playerIndex][hp_max] << "   ";
	cout << "EP: " << (long)player_stats[playerIndex][ep_current] << "/" << (long)player_stats[playerIndex][ep_max] << " (+" << player_stats[playerIndex][ep_regen] << ")" << endl;
	cout << "ATK: " << (long)player_stats[playerIndex][atk] << " | END: " << (long)player_stats[playerIndex][end_stat] << endl;
	cout << "Active Gates: ";
	if (gate_active_G1) cout << "G1 ";
	if (gate_active_G2) cout << "G2 ";
	if (!gate_active_G1 && !gate_active_G2) cout << "None";
	cout << "\n";
	cout << "----------------------------------------" << endl;
	cout << "Boss: " << (long)bossHP << "/" << (long)boss[bossPhase][boss_hp] << " (Phase " << bossPhase + 1 << ")" << endl;
	cout << "----------------------------------------" << endl;
	cout << "Choose your action:" << endl;
	cout << " 1) Attack" << endl;
	cout << " 2) Use Skill" << endl;
	cout << " 3) Toggle Gate" << endl;
	cout << "Enter action: ";
	int action_choice;
	cin >> action_choice;

	if (cin.fail()) {
		cin.clear(); 
		cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
		cout << "Invalid input. Please enter a number between 1 and 3." << endl;
		cout << " " << endl;
			
	}

	switch (action_choice) {
		case 1: {
			int damage = ApplyDamage((int)player_stats[playerIndex][atk], (int)boss[bossPhase][boss_end]);
			bossHP -= damage;
			cout << "You attack the boss and dealt " << damage << " damage!" << endl;
			cout << "" << endl;
			return true;
		}

	case 2: {
		displayskills_battle(playerIndex);
		cout << "" << endl;
		cout << "Enter the skill number to use (or 6 to Cancel): ";
		int skillChoice; 
		cin >> skillChoice;

		if (cin.fail() || skillChoice < 1 || skillChoice > 6) {
			cin.clear(); 
			cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
			cout << "Invalid input. Please enter a valid skill number." << endl;
			cout << " " << endl;
			return PlayerActionMenu(playerIndex, bossPhase, bossHP);
		}

		// Map displayed choice to actual skill array index for showing cost/cd
		int displayedSkillIdx = -1;
		switch (skillChoice) {
			case 1: displayedSkillIdx = 2; break; // Kei-ga-n Barrage
			case 2: displayedSkillIdx = 3; break; // Front Lotus
			case 3: displayedSkillIdx = 6; break; // Chain Barrage
			case 4: displayedSkillIdx = 7; break; // Reverse Lotus
			case 5: displayedSkillIdx = 8; break; // Art of Resistance
			default: displayedSkillIdx = -1; break;
		}

		// Confirm skill usage
		if (displayedSkillIdx != -1) {
			char conf;
			cout << "Confirm use of '" << skill_name[displayedSkillIdx] << "' (Cost: " << skill_cost[displayedSkillIdx] << " EP, CD: " << skill_cooldowns[displayedSkillIdx] << ")? (y/n): ";
			cin >> conf;
			if (cin.fail()) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); cout << "Invalid input. Cancelling skill." << endl; return PlayerActionMenu(playerIndex, bossPhase, bossHP); }
			if (conf != 'y' && conf != 'Y') {
				cout << "Skill use cancelled." << endl;
				cout << " " << endl;
				return PlayerActionMenu(playerIndex, bossPhase, bossHP);
			}
		}

		int skillIndex = skillChoice;
		bool skillSuccess = UseSkill(playerIndex, skillIndex, bossPhase, bossHP);
		if (!skillSuccess) {
			cout << " " << endl;
			return PlayerActionMenu(playerIndex, bossPhase, bossHP);
		}
		return true;
	}		case 3: {
		 int toggled = ToggleGates(playerIndex);
		 if (toggled == -1) {
			 cout << "Gate toggle failed or cancelled. Choose another action." << endl;
			 cout << "" << endl;
			 return PlayerActionMenu(playerIndex, bossPhase, bossHP);
		 } else if (toggled == 0) {
			 return PlayerActionMenu(playerIndex, bossPhase, bossHP);
		 }
		}

		default:
			cout << "Invalid choice. Please try again." << endl;
			cout << " " << endl;
			return PlayerActionMenu(playerIndex, bossPhase, bossHP);
	}
}

void EnemyActionMenu(int playerIndex, int &bossPhase, int &bossHP) {
	int damage = ApplyDamage((int)boss[bossPhase][boss_atk], (int)player_stats[playerIndex][end_stat]);
	player_stats[playerIndex][hp_current] -= damage;

	cout << "The boss attacks you and dealt " << damage << " damage!" << endl;
}

void EndTurn (int playerIndex, int &bossPhase, int &bossHP){
	if (bossHP <=1600 && bossPhase == base_form) {
		cout << "Arlong: Hmph! You dare scratch me, human?! I`ll show you the true terror of the seas!" << endl;
		cout << "Arlong: It`s time I got serious. Taste my Kiribachi form!" << endl;
		cin.get();
		bossPhase = kiribachi;
		bossHP = boss[bossPhase][boss_hp];
		cout << "The boss has transformed into its Kiribachi Form!" << endl;
		cin.get();
		cout << " " << endl;
	} else if (bossHP <=900 && bossPhase == kiribachi) {
		cout << "Arlong: You`ve gone too far, worm! I am the king of these waters!" << endl;
		cout << "Arlong: Now witness my TRUE power! The Enraged Form of the Sea Tyrant!" << endl;
		cin.get();
		bossPhase = enraged;
		bossHP = boss[bossPhase][boss_hp];
		cout << "The boss has transformed into its Enraged Form!" << endl;
		cin.get();
		cout << " " << endl;
	}


	if (gate_active_G1) {
		if (gate_just_activated_G1[playerIndex]) {
			gate_just_activated_G1[playerIndex] = false;
		} else {
			if (player_stats[playerIndex][ep_current] >= gate_per_turn_G1) {
				player_stats[playerIndex][ep_current] -= gate_per_turn_G1;
			} else {
				cout << "You lack EP to maintain Gate of Opening (G1). It crashes and deactivates." << endl;
				EndGate(playerIndex, 1, true);
			}
		}
	}

	if (gate_active_G2) {
		if (gate_just_activated_G2[playerIndex]) {
			gate_just_activated_G2[playerIndex] = false;
		} else {
			if (player_stats[playerIndex][ep_current] >= gate_per_turn_G2) {
				player_stats[playerIndex][ep_current] -= gate_per_turn_G2;
			} else {
				cout << "You lack EP to maintain Gate of Healing (G2). It crashes and deactivates." << endl;
				EndGate(playerIndex, 2, true);
			}
		}
	}
}

// TRAINING LOOP
void trainingloop(int playerIndex){
	int turn = 48;
	char confirm_input;
	int confirm;
	int training_choice;

	for (int i = 1; i <= turn; i++){
		cout << "*** [Turn " << i << " of " << turn << "] ***" << endl;
		cout << "------------------------------------------" << endl;
		cout << "[No. of stats trained so far]: " << endl;
		cout << "HP: " << player_training_history[playerIndex][hp_max] << " | ATK: " << player_training_history[playerIndex][atk] << " | END: " << player_training_history[playerIndex][end_stat] << " | EP: " << player_training_history[playerIndex][ep_max] << " | EP Regen: " << player_training_history[playerIndex][ep_regen] << " | Self Discovery: " << player_training_history[playerIndex][self_discovery] << endl;
		cout << "------------------------------------------" << endl;
		cout << "[Current Stats]: " << endl;
		cout << "HP: " << (long)player_stats[playerIndex][hp_max] << " | ATK: " << (long)player_stats[playerIndex][atk] << " | END: " << (long)player_stats[playerIndex][end_stat] << " | EP: " << (long)player_stats[playerIndex][ep_max] << " | EP Regen: " << fixed << setprecision(1) << player_stats[playerIndex][ep_regen]; if (player_stats[playerIndex][self_discovery] < 8) {cout << " | Self Discovery: " << (long)player_stats[playerIndex][self_discovery] << "/ ?" << endl;} else if (player_stats[playerIndex][self_discovery] >= 9) {cout << " | Self Discovery: " << "MAX"<< endl;};
		cout << "------------------------------------------" << endl;

		cout << "Choose a stat to train: \n 1. HP \n 2. ATK \n 3. END \n 4. EP \n 5. EP Regen \n 6. Self Discovery \n 7. Check skill list" << endl;
		cout << "------------------------------------------" << endl;
		cout << "Enter action: ";
		cin >> training_choice;

		if (cin.fail()) {
    	cin.clear(); 
    	cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
    	cout << "Invalid input. Please enter a number between 1 and 7." << endl;
    	cout << " " << endl;
    	i--; 
    	continue;
		}

		if (training_choice < 1 || training_choice > 7) {
    	cout << "Invalid choice. Please enter a number between 1 and 7." << endl;
    	cout << " " << endl;
    	i--; 
    	continue;
		}


		while (true){
			if (training_choice == 1){
				confirm = ApplyStatGain(hp_max, 1 + player_training_history[playerIndex][hp_max]);
				cout << "You will gain +" << confirm << " HP. Confirm? (y/n): ";
				cin >> confirm_input;
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << " " << endl;

			if (confirm_input == 'y' || confirm_input == 'Y'){
				player_stats[playerIndex][hp_max] += confirm;
				player_stats[ playerIndex][hp_current] = player_stats[playerIndex][hp_max];
				player_training_history[playerIndex][hp_max] += 1;
					cout << "You gained +" << confirm << " HP!" << endl;
					cout << " " << endl;
					cin.get();
					break;
				}
				else if (confirm_input == 'n' || confirm_input == 'N'){
					cout << "Training cancelled. Please choose again." << endl;
					cout << " " << endl;
					i--;
					break;
				}
				else {
					cout << "Invalid input. Please enter 'y' or 'n': ";
					cout << " " << endl; 
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cin >> confirm_input;
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
				}
			} else if (training_choice == 2){
				confirm = ApplyStatGain(atk, 1 + player_training_history[playerIndex][atk]);
				cout << "You will gain +" << confirm << " ATK. Confirm? (y/n): ";
				cin >> confirm_input;
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << " " << endl;

			if (confirm_input == 'y' || confirm_input == 'Y'){
				player_stats[playerIndex][atk] += confirm;
				player_training_history[playerIndex][atk] += 1;
					cout << "You gained +" << confirm << " ATK!" << endl;
					cout << " " << endl;
					cin.get();
					break;
				}
				else if (confirm_input == 'n' || confirm_input == 'N'){
					cout << "Training cancelled. Please choose again." << endl;
					cout << " " << endl;
					i--;
					break;
				}
				else {
					cout << "Invalid input. Please enter 'y' or 'n': ";
					cout << " " << endl; 
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cin >> confirm_input;
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
				}
		} else if (training_choice == 3){
				confirm = ApplyStatGain(end_stat, 1 + player_training_history[playerIndex][end_stat]);
				cout << "You will gain +" << confirm << " END. Confirm? (y/n): ";
				cin >> confirm_input;
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << " " << endl;

			if (confirm_input == 'y' || confirm_input == 'Y'){
				player_stats[playerIndex][end_stat] += confirm;
				player_training_history[playerIndex][end_stat] += 1;
					cout << "You gained +" << confirm << " END!" << endl;
					cout << " " << endl;
					cin.get();
					break;
				}
				else if (confirm_input == 'n' || confirm_input == 'N'){
					cout << "Training cancelled. Please choose again." << endl;
					cout << " " << endl;
					i--;
					break;
				}
				else {
					cout << "Invalid input. Please enter 'y' or 'n': ";
					cout << " " << endl; 
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cin >> confirm_input;
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
				}
		} else if (training_choice == 4){
				confirm = ApplyStatGain(ep_max, 1 + player_training_history[playerIndex][ep_max]);
				cout << "You will gain +" << confirm << " EP. Confirm? (y/n): ";
				cin >> confirm_input;
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << " " << endl;

			if (confirm_input == 'y' || confirm_input == 'Y'){
				player_stats[playerIndex][ep_max] += confirm;
				player_stats[playerIndex][ep_current] = player_stats[playerIndex][ep_max];
				player_training_history[playerIndex][ep_max] += 1;
					cout << "You gained +" << confirm << " EP!" << endl;
					cout << " " << endl;
					cin.get();
					break;
				}
				else if (confirm_input == 'n' || confirm_input == 'N'){
					cout << "Training cancelled. Please choose again." << endl;
					cout << " " << endl;
					i--;
					break;
				}
				else {
					cout << "Invalid input. Please enter 'y' or 'n': ";
					cout << " " << endl; 
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cin >> confirm_input;
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
				}
		} else if (training_choice == 5){
			confirm = ApplyStatGain(ep_regen, 1 + player_training_history[playerIndex][ep_regen]);
			cout << "You will gain +" << fixed << setprecision(1) << confirm << " EP Regen. Confirm? (y/n): ";
			cin >> confirm_input;
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << " " << endl;

		if (confirm_input == 'y' || confirm_input == 'Y'){
			player_stats[playerIndex][ep_regen] += confirm;
			player_training_history[playerIndex][ep_regen] += 1;
				cout << "You gained +" << fixed << setprecision(1) << confirm << " EP Regen!" << endl;
				cout << " " << endl;
				cin.get();
				break;
			}
			else if (confirm_input == 'n' || confirm_input == 'N'){
				cout << "Training cancelled. Please choose again." << endl;
				cout << " " << endl;
				i--;
				break;
			} else {
					cout << "Invalid input. Please enter 'y' or 'n': ";
					cout << " " << endl; 
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cin >> confirm_input;
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
				}
		} else if (training_choice == 6){
			if (player_stats[playerIndex][self_discovery] >= 9){
			cout << "You have reached the maximum Self Discovery level of 8 and learned all possible skills. You cannot train this stat further." << endl;
			cout << " " << endl;
			i--;
			break;
			} 

			confirm =  1;
				cout << "You will gain +" << confirm << " SD. Confirm? (y/n): ";
				cin >> confirm_input;
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << " " << endl;

			if (confirm_input == 'y' || confirm_input == 'Y'){
				selfdiscoverygain(playerIndex);
					cout << " " << endl;
					break;
				}
				else if (confirm_input == 'n' || confirm_input == 'N'){
					cout << "Training cancelled. Please choose again." << endl;
					cout << " " << endl;
					i--;
					break;
				}
				else {
					cout << "Invalid input. Please enter 'y' or 'n': ";
					cout << " " << endl; 
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cin >> confirm_input;
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
				}
			} else if (training_choice == 7){
				displayskills_training(playerIndex);
				cout << " " << endl;
				i--;
				break;
			}

		

	}
}
}

// BATTLE LOOP [FIRST DRAFT]
void battleloop(int playerIndex){
	cout << "======== BATTLE START ========" << endl;
	int bossPhase = base_form;
	int bossHP = boss[bossPhase][boss_hp];

	while(player_stats[playerIndex][hp_current] > 0 &&  boss[enraged][boss_hp] > 0){
		BeginTurn (playerIndex);
		bool actionSuccess = false;
		while (!actionSuccess) {
			actionSuccess = PlayerActionMenu (playerIndex, bossPhase, bossHP);
		}
		if (bossHP < 0){
			cout << "You have defeated Arlong!" << endl;
			cin.get();
			cout << "\nArlong staggers backward, blood dripping from his wounds." << endl;
			cin.get();
			cout << "Arlong: Impossible... defeated by a mere human...?" << endl;
			cin.get();
			cout << "You step forward, standing tall infront of Arlong, kneeling in defeat." << endl;
			cin.get();
			cout << "Without saying a word, you grab arlong by the neck." << endl;
			cin.get();
			cout << "Keigan: Karma came back to bite you in the ass, fishman." << endl;
			cin.get();
			cout << "Arlong: YOU ARROGANT HUMAN, I'LL MAKE YOU- *CRACK*" << endl;
			cin.get();
			cout << "Your grip tightens, snapping his neck." << endl;
			cin.get();
			cout << "Arlong suddenly goes silent, his body going limp." << endl;
			cin.get();
			cout << "Keigan:*Your voice indifferent* Now let's see if the sea would reclaim scum like you." << endl;
			cin.get();
			cout << "With the power of the gates still surging within you, you throw Arlong into the sea. His lifeless body becoming food for whatever monster that come across his corpse." << endl;
			break;
		}
		EnemyActionMenu (playerIndex, bossPhase, bossHP);
		if (player_stats[playerIndex][hp_current] <= 0){
			cout << "You have been defeated by Arlong!" << endl;
			cin.get();
			cout << "\nKeigan falls to one knee, barely conscious." << endl;
			cin.get();
			cout << "Arlong: Is this all you have achieved in eight years? Pathetic." << endl;
			cin.get();
			cout << "Arlong grabs you by the neck, victorious." << endl;
			cin.get();
			cout << "Arlong: Brings back memories doesn't it? Just like when I first enslaved Cocoyashi Village." << endl;
			cin.get();
			cout << "Arlong: Same situation, 8 years ago. Same outcome. You were weak then, and you are weak now." << endl;
			cout << "Keigan: (slowly going breathless) I-... I-ll...come...back... you..fish..bastard..." << endl;
			cin.get();
			cout << "Arlong: This time, I'll make sure the sea never spits you back out." << endl;
			cin.get();
			cout << "Darkness consumes your vision as his grip tightens, waiting for your body to go limp before throwing you full force back into the deep blue sea..." << endl;
			cin.get();
			break;
		}
		cin.get();
		EndTurn(playerIndex, bossPhase, bossHP);
		TickCooldownsAndStatuses(playerIndex);
	}
	
}



// ACT 1
void Act1() {
    cout << "[Act 1: THE FALL OF COCOYASHI] (Press Enter to continue...)" << endl;
    cin.get();

    cout << "You are Keigan Lee." << endl;
    cin.get();
    cout << "A prodigy in the modern world—ranked #1 in gymnastics and cheerdance, balancing a high-pressure accounting job in one of the country`s top firms." << endl;
    cin.get();
    cout << "You were admired, envied, and exhausted." << endl;
    cin.get();
    cout << "One night, after another 16-hour shift, your body gave out. You collapsed at your desk, surrounded by silence and spreadsheets." << endl;
    cin.get();
    cout << "No one noticed until morning. By then, it was too late." << endl;
    cin.get();

    cout << "Keigan (thinking): Is this... really how it ends?" << endl;
    cin.get();

    cout << "But death wasn`t the end." << endl;
    cin.get();
    cout << "You awaken, not in a hospital, nor in the afterlife, rather in a cradle. Your limbs are small. Your voice, a whisper. You`ve been reborn." << endl;
    cin.get();
    cout << "The world around you is unfamiliar. The air smells of salt. The language is strange, but the warmth is real." << endl;
    cin.get();

    cout << "This is East Blue, an ocean scattered with islands, pirates, and dreams." << endl;
    cin.get();
    cout << "Your new home is Cocoyashi Village, a peaceful coastal town where laughter still echoes through the trees." << endl;
    cin.get();

    cout << "At age four, tragedy strikes again. Your new parents, kind and humble, fall ill. Despite your efforts, they pass quietly in the night." << endl;
    cin.get();
    cout << "You inherit their modest assets, but more importantly, their love." << endl;
    cin.get();

    cout << "Genzo, the village sheriff, and his wife Bell-mere take you in. Their home becomes yours. Their strength becomes your foundation." << endl;
    cin.get();
    cout << "Bell-mere is a former marine—tough, warm, and fiercely protective. Genzo is stern but gentle, always watching over you with quiet pride." << endl;
    cin.get();

    cout << "It all began on your sixth birthday. Genzo and Bell-mere hosted a modest celebration in the garden, the scent of mikans drifting in the breeze." << endl;
    cin.get();
    cout << "Bell-mere arrived with her daughters, Nojiko and Nami. You`d heard of them, but never met them until now." << endl;
    cin.get();

    cout << "Keigan: Hey... You`re Nami, right? *You extend your hand, nervous but hopeful.*" << endl;
    cin.get();
    cout << "Nami: Yep! And you`re Keigan. Nice to meet you! *Her smile is radiant, her grip firm.*" << endl;
    cin.get();
    cout << "Nojiko: Careful, Nami. You`ll scare him off with that energy. *She teases, nudging her sister.*" << endl;
    cin.get();

    cout << "From that day on, Nami and Nojiko began visiting your house regularly. While they played, you read from your parents` library—until curiosity pulled you into their games." << endl;
    cin.get();

    cout << "One afternoon, Nami found you buried in a book titled 'Beginner`s Guide to Maps and Cartography'." << endl;
    cin.get();
    cout << "Nami: Keigan, what are you studying today?" << endl;
    cin.get();
    cout << "Keigan: Maps and cartography. Where`s Nojiko?" << endl;
    cin.get();
    cout << "Nami: Helping Bell-mere at the grove. It`s just me today. Can I read with you?" << endl;
    cin.get();

    cout << "She leaned closer, her curiosity shining. You blushed, but explained the words she struggled with. Together, you traced the lines of distant islands." << endl;
    cin.get();

    cout << "Nami: Keigan, I have this really big dream." << endl;
    cin.get();
    cout << "Keigan: What is it, Nami?" << endl;
    cin.get();
    cout << "Nami: I`ll draw the map of the world!" << endl;
    cin.get();
    cout << "Keigan: That`s a big dream indeed... I don't think one person is enough to achieve it. I`l help you. We`ll do it together." << endl;
    cin.get();

	cout << "Nami`s eyes sparkled. She grabbed your hand, squeezing it tightly, before pulling you in for a hug." << endl;
	cin.get();
    cout << "Her hug was sudden, warm. You froze, cheeks red. At the door, Bell-mere and Nojiko arrived, grinning at the sight." << endl;
    cin.get();
    cout << "Bell-mere: Looks like a love story is starting! *She laughed, loud and proud.*" << endl;
    cin.get();

    cout << "Months passed. Bell-mere`s mikan sales faltered. She cut back on food, eating only mikans so the girls wouldn`t notice." << endl;
    cin.get();
    cout << "You brought her meals from your own savings, insisting she stay healthy." << endl;
    cin.get();
    cout << "Keigan: Bell-mere-obasan, you can`t live on mikans alone. You`re family to me too." << endl;
    cin.get();
    cout << "Bell-mere: You`re too thoughtful for your age, Keigan. Thank you." << endl;
    cin.get();

    cout << "Life seemed normal, too normal. A sense of uneasiness and dread fills you for whatever reason." << endl;
    cin.get();
    cout << "One night, a villager burst into the square, breathless." << endl;
    cin.get();
    cout << "Villager: Pirates... Fishmen... they`re coming!" << endl;
    cin.get();

    cout << "The villagers panicked. Genzo rallied them. Bell-mere readied her rifle. You hid with Nami and Nojiko in the woods, heart pounding." << endl;
    cin.get();

    cout << "The sound of heavy footsteps and guttural laughter grew louder. The Fishmen pirates marched into Cocoyashi, towering over the humans." << endl;
    cin.get();
    cout << "At their head was Arlong, his jagged teeth glinting in the torchlight." << endl;
    cin.get();

    cout << "Arlong: Every adult pays 100,000 Beli. Children? 50,000. Fail to pay... and you die." << endl;
    cin.get();

    cout << "Bell-mere refused to yield. She paid for her daughters, but not herself. Arlong made her an example." << endl;
    cin.get();
    cout << "Bell-mere: Nami, Nojiko, Keigan... I love you." << endl;
    cin.get();

    cout << "The gunshot echoed. Her body fell. The village screamed. You lunged at Arlong, fists clenched." << endl;
    cin.get();
    cout << "Keigan: You bastard! She was our mother!" << endl;
    cin.get();

    cout << "Your blow connected—but it was meaningless. Arlong laughed, then threw you into the sea." << endl;
    cin.get();
    cout << "Arlong: The weak belong to the deep." << endl;
    cin.get();

    cout << "Darkness swallowed you. But the sea did not claim you." << endl;
    cin.get();
    cout << "You drifted for days, unconscious, broken, but alive." << endl;
	cin.get();
}


// ACT 2
void Act2(int playerIndex){
    cout << "[ACT 2: THE EIGHT-YEAR TRAINING ARC]" << endl;
    cin.get();

    cout << "300 meters west of Cocoyashi, a ship with a halo-marked Jolly Roger sails calmly. A lookout scans the horizon." << endl;
    cin.get();
    cout << "Lookout: Oi, there`s something in the sky! Is it a bird? A plane?" << endl;
    cin.get();
    cout << "He grabs his binoculars, eyes widening." << endl;
    cin.get();
    cout << "Lookout: It`s a kid! He`s falling into the water—300 meters east of us! Captain, what do we do?" << endl;
    cin.get();

    cout << "The captain, a bearded man in his late thirties, stands tall." << endl;
    cin.get();
    cout << "Captain: Alright mates! As the Good Heart Pirates, we don`t let a wee lad die at sea! Let`s save him!" << endl;
    cin.get();
    cout << "Crew: AYE!" << endl;
    cin.get();

    cout << "They row hard, closing the distance to the boy`s estimated landing spot. They expect wreckage, maybe a small boat, but instead find a child, unconscious and floating." << endl;
    cin.get();

    cout << "The ship`s doctor, known only as `Quack`, lifts Keigan aboard and begins treatment immediately." << endl;
    cin.get();

    cout << "Three days pass." << endl;
    cin.get();
    cout << "Keigan awakens in a dim room, sore and starving. He sees food and medicine beside him and begins to eat." << endl;
    cin.get();

    cout << "Doctor Quack stirs from the couch, hearing the clink of utensils." << endl;
    cin.get();
    cout << "Quack: So you`re up, lad. That`s good." << endl;
    cin.get();
    cout << "Keigan: How long was I out?" << endl;
    cin.get();
    cout << "Quack: Three days and nights. Eat that porridge. I`ll check your vitals after." << endl;
    cin.get();

    cout << "Keigan nods, grateful but aching. His body is bruised, but alive. The sea softened what would`ve been a fatal fall." << endl;
    cin.get();

    cout << "Later, Quack confirms Keigan`s recovery is on track. He`s allowed to walk slowly to prevent atrophy." << endl;
    cin.get();
    cout << "Keigan steps outside and is stunned to see open sea." << endl;
    cin.get();
    cout << "Keigan: Doc... where are we?" << endl;
    cin.get();
    cout << "Quack: Near an uninhabited island in East Blue. We`re docking there so you can rest. It`s too dangerous to keep you aboard while we sail." << endl;
    cin.get();

    cout << "An hour later, the crew sets up camp on the island. Keigan, now walking better, joins the captain, vice-captain, and Quack in a tent." << endl;
    cin.get();

    cout << "Captain: Hey there, lad. I`m the captain of this crew. That`s my vice-captain, and you`ve met our doc." << endl;
    cin.get();
    cout << "Keigan: Thanks for saving me. I owe you all." << endl;
    cin.get();

    cout << "Captain: We`re the Good Heart Pirates. I know, the name`s cheesy. But we don`t steal or kill innocents. That halo on our flag? It`s our promise." << endl;
    cin.get();

    cout << "Keigan nods, understanding." << endl;
    cin.get();
    cout << "Keigan: What happens to me now?" << endl;
    cin.get();
    cout << "Captain: First, tell us what happened. You came flying from Cocoyashi`s direction." << endl;
    cin.get();

    cout << "Keigan winces. The memory is fresh." << endl;
    cin.get();
    cout << "Keigan: I came from Cocoyashi. A pirate dictator named Arlong took over. He killed someone I loved... and threw me into the sea." << endl;
    cin.get();

    cout << "The tent falls silent. The crew listens, respectful." << endl;
    cin.get();
    cout << "Captain: Then live first, boy. Choose what to do with the life you still have." << endl;
    cin.get();

    cout << "Those words stick. After a few days of accompanying you on the island and making sure you're strong enough to hand your self, the Good Heart Pirates leave for their voyage and leaves you to carve your own path." << endl;
	
	cout << "After the Good Heart Pirates' departure, while exploring the island, you stumble upon a weathered scroll buried beneath a tree." << endl;
    cin.get();
    cout << "You unroll it. The title reads: 'The Eight Inner Gates'." << endl;
    cin.get();
    cout << "Inside are ancient techniques—body conditioning that channels life force into power." << endl;
    cin.get();

    cout << "Keigan (thinking): This... this might be the key. To surviving. To returning. To defeating Arlong." << endl;
    cin.get();

    cout << "You clench your fists. The pain is still there. But so is your resolve." << endl;
    cin.get();
    cout << "This marks the beginning of your training. To return to Cocoyashi Village. To reunite with Nami. To end Arlong`s tyranny." << endl;
    cin.get();

    trainingloop(playerIndex);
	display_final_stats(playerIndex);
}


// ACT 3
void Act3(int playerIndex){
	cout << "[ACT 3: Return to Cocoyashi (The Arlong Battle)]" << endl;
	cin.get();
	cout << "========================================" << endl;
	cout << "EIGHT YEARS LATER" << endl;
	cout << "========================================" << endl;
	cin.get();

	cout << "The seas are vast and endless. You`ve trained for eight years on that forgotten island, driven by a single purpose." << endl;
	cin.get();
	cout << "Your hands are calloused. Your body is honed. Bell-mère`s faded headband is tied around your waist—a constant reminder." << endl;
	cin.get();

	cout << "Your journey takes you to Baratie, a floating restaurant where pirates and marines alike gather." << endl;
	cin.get();
	cout << "The air crackles with chaos as you enter. A crew moves through the chaos like water—effortless, dangerous." << endl;
	cin.get();
	cout << "Your eyes land on their navigator. Sharp-eyed. Guarded. There`s something familiar in her stance, but eight years blur memory." << endl;
	cin.get();
	cout << "You keep your distance." << endl;
	cin.get();

	cout << "Then Don Krieg arrives with his armada. Cannons fire. Steel clashes. In the confusion, a legendary swordsman named Mihawk appears—and tests a green-haired pirate named Zoro." << endl;
	cin.get();
	cout << "Zoro falls, bloodied but unbroken. His captain remains." << endl;
	cin.get();
	cout << "When the dust settles, the navigator is gone. You hear her name for the first time in eight years, spoken by the fleeing pirates." << endl;
	cin.get();

	cout << "\"NAMI!\" they shout." << endl;
	cin.get();

	cout << "Your heart stops." << endl;
	cin.get();
	cout << "Keigan (to himself): Nami...?" << endl;
	cin.get();

	cout << "========================================" << endl;
	cout << "THE PURSUIT" << endl;
	cout << "========================================" << endl;
	cin.get();

	cout << "You find yourself offering to guide the Straw Hat crew—Zoro, Usopp, and others—toward the Conomi Islands." << endl;
	cin.get();
	cout << "Your instincts, buried for eight years, remember the way. They trust you without knowing why." << endl;
	cin.get();

	cout << "As your small vessel cuts through the waters, Zoro speaks first." << endl;
	cin.get();
	cout << "Zoro: So... you know where she`s heading?" << endl;
	cin.get();
	cout << "Keigan: Cocoyashi Village. It`s the only place she would go." << endl;
	cin.get();
	cout << "Usopp: How can you be so sure?" << endl;
	cin.get();
	cout << "Keigan: Because... she`s from there. And if she stole that treasure, she has a reason. A dream." << endl;
	cin.get();

	cout << "Zoro studies you for a long moment, then nods. He asks no more questions." << endl;
	cin.get();

	cout << "========================================" << endl;
	cout << "COCOYASHI VILLAGE" << endl;
	cout << "========================================" << endl;
	cin.get();

	cout << "The village is hollowed by fear. Boarded shops. Wary eyes. Armed sentries patrol the streets." << endl;
	cin.get();
	cout << "You recognize the insignia, Arlong`s fishmen." << endl;
	cin.get();

	cout << "The Straw Hat captain, Luffy, arrives with his cook Sanji. They stand united with Zoro and Usopp before a woman with orange hair and a weary expression." << endl;
	cin.get();
	cout << "It`s Nojiko." << endl;
	cin.get();

	cout << "She tells them everything: Bell-mère`s death. The tribute system. Nami`s forced enslavement. Eight years under Arlong`s boot." << endl;
	cin.get();

	cout << "Nojiko`s voice doesn`t break, but her hands do. When she finishes, she mentions an old name." << endl;
	cin.get();
	cout << "Nojiko: There was a boy... Keigan. Arlong threw him into the sea eight years ago. We thought he was dead." << endl;
	cin.get();

	cout << "The crew turns to you." << endl;
	cin.get();

	cout << "Usopp: You`re Keigan? The one Arlong tossed in the water?" << endl;
	cin.get();
	cout << "Keigan: I survived." << endl;
	cin.get();

	cout << "Zoro: And you came back." << endl;
	cin.get();
	cout << "Keigan: I promised I would." << endl;
	cin.get();

	cout << "Luffy steps forward, grinning." << endl;
	cin.get();
	cout << "Luffy: So you`re here to help us free this village?" << endl;
	cin.get();
	cout << "Keigan: I`m here to end Arlong. The rest will follow." << endl;
	cin.get();

	cout << "Sanji lights a cigarette." << endl;
	cin.get();
	cout << "Sanji: Then let`s not waste time. That bastard`s paid for eight years already." << endl;
	cin.get();

	cout << "========================================" << endl;
	cout << "THE MARCH TO ARLONG PARK" << endl;
	cout << "========================================" << endl;
	cin.get();

	cout << "Your group moves through Cocoyashi like a storm. Corrupt marines try to block the way—they fall." << endl;
	cin.get();
	cout << "Fishmen sentries attempt to stop you—they scatter." << endl;
	cin.get();
	cout << "There is no negotiation. There is only purpose." << endl;
	cin.get();

	cout << "As you approach the gates of Arlong Park, the weight of eight years settles on your shoulders." << endl;
	cin.get();
	cout << "Every training session. Every scar earned. Every moment of pain—it all leads here." << endl;
	cin.get();

	cout << "Keigan (under his breath): This time, I bring what I should have had eight years ago." << endl;
	cin.get();

	cout << "The gates open." << endl;
	cin.get();
	cout << "Arlong waits inside, his silhouette towering against the dying light." << endl;
	cin.get();

	cout << "A gravelly voice echoes from the darkness." << endl;
	cin.get();
	cout << "Arlong: Keigan Lee... I wondered if you would ever find the courage to return." << endl;
	cin.get();
	cout << "Keigan: I`ve spent eight years perfecting what should have protected Cocoyashi from the start." << endl;
	cin.get();
	cout << "Arlong: Eight years? *laughs* Boy, I ruled Cocoyashi unchallenged for eight years." << endl;
	cin.get();
	cout << "Keigan: Then your reign ends tonight." << endl;
	cin.get();
	cout << "Arlong: We shall see if your training was worth the wait... or merely an exercise in futility." << endl;
	cin.get();

	cout << "The moment of reckoning has arrived." << endl;
	cin.get();

	cout << "========================================" << endl;
	cout << "BATTLE WITH ARLONG BEGINS" << endl;
	cout << "========================================" << endl;
	cin.get();

	battleloop(playerIndex);
}

// ACT 4
void Act4(int playerIndex, bool victory) {
	if (victory) {
		cout << "========================================" << endl;
		cout << "ACT 4 - COCOYASHI'S FREEDOM AND BEYOND" << endl;
		cout << "========================================" << endl;
		cin.get();

		cout << "[Scene 1: The Ruins of Arlong Park]" << endl;
		cin.get();

		cout << "Arlong Park crumbles around you. Dust and debris settle as the roar of battle fades." << endl;
		cin.get();
		cout << "The chains that bound Cocoyashi for eight years lie shattered on the ground." << endl;
		cin.get();

		cout << "Villagers pour from their homes, their faces showing disbelief, then joy, then tears." << endl;
		cin.get();
		cout << "Your body screams in pain. The Gates extracted a price—muscles burning, breath shallow—but Arlong is gone." << endl;
		cin.get();

		cout << "Luffy appears through the smoke, grinning like an idiot despite his own wounds." << endl;
		cin.get();
		cout << "Luffy: You did it, Keigan! That bastard`s finally done!" << endl;
		cin.get();
		cout << "He drags you up with one arm, casually brushing off debris like it`s nothing." << endl;
		cin.get();

		cout << "Luffy: Come on, let`s get out of here before this whole place collapses!" << endl;
		cin.get();

		cout << "As the Straw Hats regroup and the village emerges from the shadows, you catch sight of Nami." << endl;
		cin.get();
		cout << "Her eyes are wide—relief, worry, disbelief all mixed together." << endl;
		cin.get();

		cout << "Nami: Keigan... you..." << endl;
		cin.get();
		cout << "Her voice trembles." << endl;
		cin.get();

		cout << "You manage a weak smile." << endl;
		cin.get();
		cout << "Keigan: I promised I`d come back." << endl;
		cin.get();

		cout << "========================================" << endl;
		cout << "[Scene 2: Bell-m`ere's Grave - Morning After]" << endl;
		cout << "========================================" << endl;
		cin.get();

		cout << "The sun rises over Cocoyashi, and for the first time in eight years, there are no chains." << endl;
		cin.get();
		cout << "At Bell-m`ere's grave, Nami and Keigan stand side by side." << endl;
		cin.get();
		cout << "Nojiko watches nearby with a knowing smile. Genzo stands respectfully back." << endl;
		cin.get();

		cout << "Nami places a bouquet of tangerine blossoms on the grave." << endl;
		cin.get();
		cout << "Nami: She`d be proud. Both of you." << endl;
		cin.get();

		cout << "Keigan bows deeply, his hand touching the earth." << endl;
		cin.get();
		cout << "Keigan: Bell-m`ere... the village is free. I swear it will never be stolen again." << endl;
		cin.get();

		cout << "There is no grand declaration, only quiet understanding between two children of the same home." << endl;
		cin.get();

		cout << "Nojiko steps up, teasing gently." << endl;
		cin.get();
		cout << "Nojiko: Look at you two. Eight years and you haven`t changed one bit." << endl;
		cin.get();
		cout << "She nudges Nami, who blushes." << endl;
		cin.get();

		cout << "For a moment, the weight lifts. They smile together under the morning sun." << endl;
		cin.get();

		cout << "========================================" << endl;
		cout << "[Scene 3: Three Days of Festival]" << endl;
		cout << "========================================" << endl;
		cin.get();

		cout << "The village erupts in celebration. Vendors set up stalls, music fills the air, and laughter echoes for the first time in memory." << endl;
		cin.get();
		cout << "Genzo oversees repairs. Nojiko helps Doctor Nako tend the wounded. The Straw Hats rest, eating and recovering." << endl;
		cin.get();

		cout << "On the third day, as the festival winds down, Nami visits Doctor Nako's clinic." << endl;
		cin.get();
		cout << "Doctor Nako: The tattoo on your wrist will fade with time. The physical wounds will heal." << endl;
		cin.get();
		cout << "Nami nods, looking at the mark of her captivity one last time." << endl;
		cin.get();

		cout << "Later, at Bell-m`ere's grave, Genzo and Keigan stand in solemn reflection." << endl;
		cin.get();
		cout << "Genzo: Thank you for coming back. For ending this." << endl;
		cin.get();
		cout << "Keigan: It was the only promise I had left to keep." << endl;
		cin.get();

		cout << "Luffy`s voice suddenly breaks the moment, loud and unmissable." << endl;
		cin.get();
		cout << "Luffy: Oi! Everyone! The ship`s about to sail! Stop being sad and let`s go!" << endl;
		cin.get();

		cout << "The mood shifts instantly. Usopp laughs, Sanji rolls his eyes, Zoro nods approvingly." << endl;
		cin.get();
		cout << "Genzo smiles despite himself. This is how it should be." << endl;
		cin.get();

		cout << "========================================" << endl;
		cout << "[Scene 4: The Docks - Recruitment]" << endl;
		cout << "========================================" << endl;
		cin.get();

		cout << "At Cocoyashi Docks, the Going Merry sways gently on the water." << endl;
		cin.get();
		cout << "Nami stands at the bow, her navigation tools packed, her dreams finally within reach." << endl;
		cin.get();

		cout << "Luffy spots you in the crowd and grins." << endl;
		cin.get();
		cout << "Luffy: Oi, Keigan! You`re strong — and you care about this village. Come with us!" << endl;
		cin.get();

		cout << "The world seems to pause." << endl;
		cin.get();
		cout << "Usopp: He`s serious. When Luffy asks, he always means it." << endl;
		cin.get();
		cout << "Sanji: You`ll need someone to handle the finances anyway. This crew spends like idiots." << endl;
		cin.get();
		cout << "Zoro: Besides, you`ve got the strength. That counts." << endl;
		cin.get();

		cout << "Nami looks down at you from the deck, her expression unreadable but her eyes hopeful." << endl;
		cin.get();

		cout << "You smile—truly smile—for the first time since you returned." << endl;
		cin.get();
		cout << "Keigan: Guess I`ll handle the bookkeeping, then." << endl;
		cin.get();

		cout << "The Straw Hats erupt in cheers. Nami`s face brightens, and she extends her hand to help you aboard." << endl;
		cin.get();

		cout << "Genzo watches from the shore, Nojiko beside him, both smiling." << endl;
		cin.get();
		cout << "Nojiko: She`s been waiting for this. For him." << endl;
		cin.get();
		cout << "Genzo: That girl finally gets to chase her dream. And he gets his second chance." << endl;
		cin.get();

		cout << "========================================" << endl;
		cout << "[Scene 5: On the Open Sea - Bounty Reveal]" << endl;
		cout << "========================================" << endl;
		cin.get();

		cout << "Days later, the Going Merry cuts through calm waters." << endl;
		cin.get();
		cout << "Nami lounges on deck, reading the newspaper, when a sheet flutters loose." << endl;
		cin.get();
		cout << "You catch it before it hits the floor." << endl;
		cin.get();

		cout << "Wanted posters. Two of them." << endl;
		cin.get();

		cout << "\"Straw Hat Monkey D. Luffy — 30,000,000 Beli\"" << endl;
		cin.get();
		cout << "\"Jet Lee Keigan — 5,000,000 Beli\"" << endl;
		cin.get();

		cout << "The crew gathers around." << endl;
		cin.get();
		cout << "Usopp: Jet? What, you planning to fly next time?" << endl;
		cin.get();
		cout << "Sanji: Not bad for a first bounty. Shows the Marines are paying attention." << endl;
		cin.get();
		cout << "Zoro: Weak. But it`s a start." << endl;
		cin.get();

		cout << "You laugh—a genuine, free laugh. For the first time, your name carries weight across the seas." << endl;
		cin.get();
		cout << "The world now knows who you are." << endl;
		cin.get();

		cout << "Nami smirks from her position on the deck." << endl;
		cin.get();
		cout << "Nami: Welcome to the crew, bookkeeper. Try not to give us all a heart attack with your bounty next time." << endl;
		cin.get();

		cout << "Luffy grins impossibly wide." << endl;
		cin.get();
		cout << "Luffy: I`m gonna be King of the Pirates! And you`re all gonna be legends with me!" << endl;
		cin.get();

		cout << "The sea stretches endlessly before you, full of promise and adventure." << endl;
		cin.get();
		cout << "For the first time in your life, the future feels bright." << endl;
		cin.get();

		cout << "========================================" << endl;
		cout << "THE END - VICTORY" << endl;
		cout << "========================================" << endl;
		cin.get();

	} else {
		cout << "========================================" << endl;
		cout << "ACT 4 - THE DREAM'S END" << endl;
		cout << "========================================" << endl;
		cin.get();

		cout << "Darkness." << endl;;
		cin.get();
		cout << "The sea—cold and merciless—catches you once more." << endl;
		cin.get();

		cout << "As you sink deeper, the world above fades. Voices become muffled. Light becomes memory." << endl;
		cin.get();
		cout << "You think of Nami. You think of Bell-m`ere." << endl;
		cin.get();
		cout << "You think of the promise you made eight years ago." << endl;
		cin.get();

		cout << "\"I`m sorry,\" you whisper to the abyss." << endl;
		cin.get();

		cout << "The water is everywhere." << endl;
		cin.get();
		cout << "Everything fades to black." << endl;
		cin.get();

		cout << "========================================" << endl;
		cin.get();

		cout << "Your eyes snap open." << endl;
		cin.get();
		cout << "You`re six years old." << endl;
		cin.get();
		cout << "You`re standing outside Bell-m`ere`s house in Cocoyashi Village." << endl;
		cin.get();
		cout << "The sun is shining. The village is whole. Arlong never came." << endl;
		cin.get();

		cout << "But something is wrong." << endl;
		cin.get();
		cout << "A dread, heavy and inexplicable, settles in your chest." << endl;
		cin.get();

		cout << "Keigan (to yourself): Was it... all a dream?" << endl;
		cin.get();
		cout << "Keigan: Can I really defeat Arlong?" << endl;
		cin.get();

		cout << "The vision wavers. Colors blur." << endl;
		cin.get();
		cout << "In the distance, you hear laughter. Nami`s laughter." << endl;
		cin.get();
		cout << "But it fades, fades, fades..." << endl;
		cin.get();

		cout << "Into silence." << endl;
		cin.get();

		cout << "========================================" << endl;
		cout << "THE END - DEFEAT (RESET)" << endl;
		cout << "========================================" << endl;
		cin.get();
	}
}

// MAIN GAME INITIALIZATION
void InitGame(){
	cout << "Initializing..." << endl;
	cin.get();
	int playerIndex = 0;
	Act1();
	Act2(playerIndex);
	Act3(playerIndex);
	
	if (player_stats[playerIndex][hp_current] > 0) {
		Act4(playerIndex, true);
	} else {
		Act4(playerIndex, false); 
	}
}

void TrainingDemo(){
	cout << "========================================" << endl;
	cout << "TRAINING DEMO" << endl;
	cout << "========================================" << endl;
	cin.get();
	
	int playerIndex = 0;
	trainingloop(playerIndex);
	cout<< "========================================" << endl;
	cout << "FINAL STATS AFTER TRAINING" << endl;
	cout<< "========================================" << endl;
	display_final_stats(playerIndex);
	cout << "========================================" << endl;
	cout << "END OF DEMO" << endl;
	cout << "========================================" << endl;
	cin.get();
}

void BattleDemo(){
	cout << "========================================" << endl;
	cout << "BATTLE DEMO" << endl;
	cout << "========================================" << endl;
	cin.get();
	
	int playerIndex = 0;

	player_stats[playerIndex][hp_max] = 400;
	player_stats[playerIndex][hp_current] = 400;
	player_stats[playerIndex][atk] = 400;
	player_stats[playerIndex][end_stat] = 450;
	player_stats[playerIndex][ep_max] = 150;
	player_stats[playerIndex][ep_current] = 150;
	player_stats[playerIndex][ep_regen] = 35;
	player_stats[playerIndex][self_discovery] = 9;
	
	for (int i = 0; i < skillCount; i++) {
		player_skills[playerIndex][i] = 1;
	}
	
	cout << "Demo stats initialized. Prepare for battle!" << endl;
	cin.get();
	
	battleloop(playerIndex);
	cout << "========================================" << endl;
	cout << "END OF DEMO" << endl;
	cout << "========================================" << endl;
	cin.get();
}


// MAIN MENU
int main (){
	int choice;
	cout << "Welcome to One Piece: The Cocoyashi Arc - Keigan's Journey!" << endl;
	cout << "What would you like to do? \n 1. New Game \n 2. Training Demo \n 3. Battle Demo \n 4. Quit" << endl;
	cout << "Enter action: ";
	cin >> choice;

	while (true){
		if (choice == 1){
			cout << "Starting New Game..." << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			InitGame();
			break;
		}
		else if (choice == 2){
			cout << "Starting Training Demo..." << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			TrainingDemo();
			break;
		}
		else if (choice == 3){
			cout << "Starting Battle Demo..." << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			BattleDemo();
			break;
		}
		else if (choice == 4){
			cout << "Quitting Game. Goodbye!" << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			return 0;
		}
		else {
			cout << "Invalid choice. Please enter 1, 2, 3, or 4: ";
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cin >> choice;
		}
	}
}