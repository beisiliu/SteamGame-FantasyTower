#pragma once
// 64 * 64 ͼƬͨ��ͼƬ����
constexpr auto PNG_USUAL_WIDTH64 = 64;
constexpr auto PNG_USUAL_HEIGHT64 = 64;
constexpr auto PNG_USUAL_WIDTH32 = 32;
constexpr auto PNG_USUAL_HEIGHT32 = 32;

// ͨ�ý�����Ĵ�С
constexpr auto AWARD_USUAL_FRAME_WIDTH = 256;
constexpr auto AWARD_USUAL_FRAME_HEIGHT = 128;

// SDL2
//�������ļ�·��
constexpr auto FILE_GAME_SETTING_PATH = "config/setting.bin";  // ��Ϸ���������ļ�·��
constexpr auto FILE_GAME_SAVE_PATH = "save/save01.bin"; // ��Ϸ���ݴ洢
constexpr auto TTF_FONT_SIZE16 = 16; // �����С
constexpr auto TTF_FONT01_PAHT = "ttf/font01.ttf";  // ��Ϸ�����ļ�·��
constexpr auto FILE_LANGUAGE_EN_SETTING_PATH = "config/startScreenLanguage_en.bin";  // Ӣ�����԰�
constexpr auto FILE_LANGUAGE_CH_SETTING_PATH = "config/startScreenLanguage_ch.bin";  // �������԰�
constexpr auto MP3_MAIN_PATH = "music/01.mp3"; // �����ļ�

//������ͼƬ·����ͼƬID
// ��ʼ����
//�󱳾�ͼƬ
constexpr auto PNG_BACKGROUND_PATH = "img/BackGround.bin";
constexpr auto PNG_BACKGROUND_ID = "PNG_BACKGROUND_ID";
//�ƶ�
constexpr auto PNG_BACKGROUND_CLOUD_PATH = "img/BackGround_Cloud.bin";
constexpr auto PNG_BACKGROUND_CLOUD_ID = "PNG_BACKGROUND_CLOUD_ID";
//HT��ͼ
constexpr auto PNG_BACKGROUND_HT_PATH = "img/BackGround_HT.bin";
constexpr auto PNG_BACKGROUND_HT_ID = "PNG_BACKGROUND_HT_ID";
//��ϷĿ������
constexpr auto TTF_GAMEPURPOSE_ID = "TTF_GAMEPURPOSE_ID";
constexpr auto TTF_GAMEPURPOSE_INFOR_ID = "TTF_GAMEPURPOSE_INFOR_ID";
//ȫ����ť
constexpr auto PNG_FULLWINDOW_PATH = "img/FullWindow.bin";
constexpr auto PNG_FULLWINDOW_ID = "PNG_FULLWINDOW_ID";


// ������
// ����ͷ��animationͼƬ����������̬�Ͷ�̬
constexpr auto PNG_ANIMATION_PERSON_PATH = "img/Animation_Person.bin";
constexpr auto PNG_ANIMATION_PERSON_ID = "PNG_ANIMATION_PERSON_ID";
// ���ͼƬ
constexpr auto PNG_PERSON_GOLD_PATH = "img/GameGold.bin";
constexpr auto PNG_PERSON_GOLD_ID = "PNG_PERSON_GAMEGOLD_ID";
// ʱ��ͼƬ
constexpr auto PNG_GAME_TIME_PATH = "img/GameTime.bin";
constexpr auto PNG_GAME_TIME_ID = "PNG_GAME_TIME_ID";
// ��������
constexpr auto TTF_PLAYER_NAME_ID = "TTF_PLAYER_NAME_ID";
constexpr auto TTF_PLAYER_NAME_X = 64;  // ��������ͷ��animationͼƬ��X���ض���
constexpr auto TTF_PLAYER_NAME_Y = 0;  // ��������ͷ��animationͼƬ��Y���ض���
constexpr auto TTF_PLAYER_NAME_HEIGHT = 64; // ��������ͷ��animationͼƬ��height����
// HT������,HTͼƬ,������ť,�����ʵ�ʷֱ��ʽ�������
constexpr auto PNG_HT_BUTTON_PATH = "img/Button_HT.bin";
constexpr auto PNT_HT_BUTTON_ID = "PNT_HT_BUTTON_ID"; 
// ��Ϸ����
constexpr auto TTF_GAME_FLOOR_ID = "TTF_GAME_FLOOR_ID";	// ���֣���

// ս������
// HTս������
constexpr auto PNG_BACKGROUND_HT_ATTACK_PATH = "img/BackGroundHT_Attack.bin";
constexpr auto PNG_BACKGROUND_HT_ATTACK_ID = "PNG_BACKGROUND_HT_ATTACK_ID";
constexpr auto PNG_GAMEWIN_PATH = "img/GameWin.bin";
constexpr auto PNG_GAMEWIND_ID = "PNG_GAMEWIND_ID";
constexpr auto PNG_GAMELOSE_PATH = "img/GameLose.bin";
constexpr auto PNG_GAMELOSE_ID = "PNG_GAMELOSE_ID";
// HTս������ͼƬ
constexpr auto ANIMATIONHT_ATTACK_PATH = "img/Animation_Attack.bin";
constexpr auto ANIMATIONHT_ATTACK_ID = "ANIMATIONHT_ATTACK_ID";
// HTһ�����ս������ͼƬ
constexpr auto ANIMATIONHT_ENEMY_FLOOR001_PATH = "img/Animation_EnemyFloor001.bin";
constexpr auto ANIMATIONHT_ENEMY_FLOOR001_ID = "ANIMATIONHT_ENEMY_FLOOR001_ID";
constexpr auto ENEMY_FLOOR001NAME_ID = "ENEMY_FLOOR001NAME_ID";
// HT�������ͼƬ
constexpr auto ENEMY_FLOOR002_PATH = "img/EnemyFloor002.bin";
constexpr auto ENEMY_FLOOR002_ID = "ENEMY_FLOOR002_ID";
constexpr auto ENEMY_FLOOR002NAME_ID = "ENEMY_FLOOR002NAME_ID";
// HT�������ͼƬ
constexpr auto ENEMY_FLOOR003_PATH = "img/EnemyFloor003.bin";
constexpr auto ENEMY_FLOOR003_ID = "ENEMY_FLOOR003_ID";
constexpr auto ENEMY_FLOOR003NAME_ID = "ENEMY_FLOOR003NAME_ID";
// HT�Ĳ����ͼƬ
constexpr auto ENEMY_FLOOR004_PATH = "img/EnemyFloor004.bin";
constexpr auto ENEMY_FLOOR004_ID = "ENEMY_FLOOR004_ID";
constexpr auto ENEMY_FLOOR004NAME_ID = "ENEMY_FLOOR004NAME_ID";
// HT������ͼƬ
constexpr auto ENEMY_FLOOR005_PATH = "img/EnemyFloor005.bin";
constexpr auto ENEMY_FLOOR005_ID = "ENEMY_FLOOR005_ID";
constexpr auto ENEMY_FLOOR005NAME_ID = "ENEMY_FLOOR005NAME_ID";
// HT�������ͼƬ
constexpr auto ENEMY_FLOOR006_PATH = "img/Animation_EnemyFloor006.bin";
constexpr auto ENEMY_FLOOR006_ID = "ENEMY_FLOOR006_ID";
constexpr auto ENEMY_FLOOR006NAME_ID = "ENEMY_FLOOR006NAME_ID";
// HT 7floor Enemy pic
constexpr auto ENEMY_FLOOR007_PATH = "img/Animation_EnemyFloor007.bin";
constexpr auto ENEMY_FLOOR007_ID = "ENEMY_FLOOR007_ID";
constexpr auto ENEMY_FLOOR007NAME_ID = "ENEMY_FLOOR007NAME_ID";
// HT chests 
constexpr auto ANIMATION_CHESTS_PATH = "img/Animation_Chests.bin";
constexpr auto ANIMATION_CHESTS_ID = "ANIMATION_CHESTS_ID";
// Props
constexpr auto PNG_PRIMARYSTRENGTHENINGSTONE_PATH = "img/PrimaryStrengtheningStone.bin";
constexpr auto PNG_PRIMARYSTRENGTHENINGSTONE_ID = "PNG_PRIMARYSTRENGTHENINGSTONE_ID";

// �����ƶ�����X��λ��
constexpr auto ENEMY_ANIMATION_MOVE_X = 32;

// ��԰����
// �󱳾�ͼ
constexpr auto PNG_BACKGROUND_HOME_PATH = "img/BackGround_Home.bin";
constexpr auto PNG_BACKGROUND_HOME_ID = "PNG_BACKGROUND_HOME_ID";
// ˯������ͼ
constexpr auto PNG_BACKGROUND_HOME_SLEEP_PATH = "img/BackGround_Sleep.bin";
constexpr auto PNG_BACKGROUND_HOME_SLEEP_ID = "PNG_BACKGROUND_HOME_SLEEP_ID";
constexpr auto PNG_BACKGROUND_HOME_SLEEPZ_PATH = "img/BackGround_SleepZ.bin";
constexpr auto PNG_BACKGROUND_HOME_SLEEPZ_ID = "PNG_BACKGROUND_HOME_SLEEPZ_ID";

// ѧԺ����
// �󱳾�ͼ
constexpr auto PNG_BACKGROUND_SCHOOL_PATH = "img/BackGround_School.bin";
constexpr auto PNG_BACKGROUND_SCHOOL_ID = "PNG_BACKGROUND_SCHOOL_ID";
// ѵ������ͼ
constexpr auto ANIMATION_PERSON_SCHOOL_PATH = "img/Animation_PersonSchool.bin";
constexpr auto ANIMATION_PERSON_SCHOOL_ID = "ANIMATION_PERSON_SCHOOL_ID";

// �̵곡��
// �󱳾�ͼ
constexpr auto PNG_BACKGROUND_SHOP_PATH = "img/BackGround_Shop.bin";
constexpr auto PNG_BACKGROUND_SHOP_ID = "PNG_BACKGROUND_SHOOP_ID";
// �򹤱���ͼ
constexpr auto PNG_BACKGROUND_SHOPWORK_PATH = "img/BackGround_ShopWork.bin";
constexpr auto PNG_BACKGROUND_SHOPWORK_ID = "PNG_BACKGROUND_SHOPWORK_ID";

// �ɾͳ���
constexpr auto PNG_ACHIEVEMENTS_STAR_PATH = "img/AchievementStar.bin";
constexpr auto PNG_ACHIEVEMENTS_STAR_ID = "PNG_ACHIEVEMENTS_STAR_ID";
constexpr auto PNG_ACHIEVEMENTS_NOSTAR_PATH = "img/AchievementNoStar.bin";
constexpr auto PNG_ACHIEVEMENTS_NOSTAR_ID = "PNG_ACHIEVEMENTS_NOSTAR_ID";

// ��־����
constexpr auto PNG_LOG_ENEMY_FLOOR001_PAHT = "img/LogEnemyFloor001.bin";
constexpr auto PNG_LOG_ENEMY_FLOOR001_ID = "PNG_LOG_ENEMY_FLOOR001_ID";
constexpr auto PNG_LOG_ENEMY_FLOOR006_PAHT = "img/LogEnemyFloor006.bin";
constexpr auto PNG_LOG_ENEMY_FLOOR006_ID = "PNG_LOG_ENEMY_FLOOR006_ID";
constexpr auto PNG_LOG_ENEMY_FLOOR007_PAHT = "img/LogEnemyFloor007.bin";
constexpr auto PNG_LOG_ENEMY_FLOOR007_ID = "PNG_LOG_ENEMY_FLOOR007_ID";
constexpr auto PNG_LOG_CHESTS_PATH = "img/LogChests.bin";
constexpr auto PNG_LOG_CHESTS_ID = "PNG_LOG_CHESTS_ID";

// װ��ͼ��
constexpr auto PNG_BEGINEERSWORD_PATH = "img/equipments/BEGINNERSWORD.bin";
constexpr auto PNG_BEGINEERSWORD_ID = "PNG_BEGINEERSWORD_ID";
constexpr auto PNG_BEGINNERHELMET_PATH = "img/equipments/BEGINNERHELMET.bin";
constexpr auto PNG_BEGINNERHELMET_ID = "PNG_BEGINNERHELMET_ID";
constexpr auto PNG_BEGINNERBARCERS_PATH = "img/equipments/BEGINNERBARCERS.bin";
constexpr auto PNG_BEGINNERBARCERS_ID = "PNG_BEGINNERBARCERS_ID";
constexpr auto PNG_BEGINNERBREASTPLATE_PATH = "img/equipments/BEGINNERBREASTPLATE.bin";
constexpr auto PNG_BEGINNERBREASTPLATE_ID = "PNG_BEGINNERBREASTPLATE_ID";
constexpr auto PNG_BEGINNERKNEEPAD_PATH = "img/equipments/BEGINNERKNEEPAD.bin";
constexpr auto PNG_BEGINNERKNEEPAD_ID = "PNG_BEGINNERKNEEPAD_ID";

// ����ͼ��
constexpr auto PNG_SKILL_DEFENSE_PATH = "img/skills/Defense.bin";
constexpr auto PNG_SKILL_DEFENSE_ID = "PNG_SKILL_DEFENSE_ID";
constexpr auto PNG_SKILL_HEAVYATTACK_PATH = "img/skills/HeavyAttack.bin";
constexpr auto PNG_SKILL_HEAVYATTACK_ID = "PNG_SKILL_HEAVYATTACK_ID";

// ҩˮͼ��
constexpr auto PNG_POTION_SMALLHEALTHPOTION_PATH = "img/potions/SmallHealthPotion.bin";
constexpr auto PNG_POTION_SMALLHEALTHPOTION_ID = " PNG_POTION_SMALLHEALTHPOTION_ID";
constexpr auto PNG_POTION_SMALLSTATUSPOTION_PATH = "img/potions/SmallStatusPotion.bin";
constexpr auto PNG_POTION_SMALLSTATUSPOTION_ID = "PNG_POTION_SMALLSTATUSPOTION_ID";

// ImGui
// ��ʼ���� 
// ����ѡ���ı���Ϳ�ʼ��Ϸ�ı����С��λ��ͨ���������õ���
constexpr auto WINDOWPOS_LANGUAGE_WIDTH = 128;
constexpr auto WINDOWPOS_LANGUAGE_HEIGHT = 64;
constexpr auto WINDOWPOS_STARTGAME_WIDTH = 128;
constexpr auto WINDOWPOS_STARTGAME_HEIGHT = 128;
constexpr auto WINDOWPOS_USERNAME_WIDTH = 320; 
constexpr auto WINDOWPOS_USERNAME_HEIGHT = 128;
// ������
// ���˵����С,λ��ͨ���������õ���
constexpr auto WINDOWSIZE_MENU_WIDTH = 960;
constexpr auto WINDOWSIZE_MENU_HEIGHT = 48;
// ս������
constexpr auto ATTACK_FRAME_HEIGHT = 128.0f;

// ͨ�ð�ť���С
constexpr auto BUTTONSIZE_USUAL128_WIDTH = 128;
constexpr auto BUTTONSIZE_USUAL96_WIDTH = 96;
constexpr auto BUTTONSIZE_USUAL32_HEIGHT = 32;

// ����5�������ƶ���, ��Ҫ��json�ļ���Ӧ
constexpr auto BEGINNERSWORD = "BeginnerSword";
constexpr auto BEGINNERHELMET = "BeginnerHelmet";
constexpr auto BEGINNERBARCERS = "BeginnerBarcers";
constexpr auto BEGINNERBREASTPLATE = "BeginnerBreastplate";
constexpr auto BEGINNERKNEEPAD = "BeginnerKneepad";


// ���ܣ���Ҫ��json�ļ���key��Ӧ
constexpr auto SKILLDEFENSE = "Defense";
constexpr auto SKILLHEAVYATTACK = "HeavyAttack";
