# ----------------------------------------------------------------- #
#           MMDAgent "Sample Script"                                #
#           released by MMDAgent Project Team                       #
#           http://www.mmdagent.jp/                                 #
# ----------------------------------------------------------------- #
#                                                                   #
#  Copyright (c) 2009-2015  Nagoya Institute of Technology          #
#                           Department of Computer Science          #
#                                                                   #
# Some rights reserved.                                             #
#                                                                   #
# This work is licensed under the Creative Commons Attribution 3.0  #
# license.                                                          #
#                                                                   #
# You are free:                                                     #
#  * to Share - to copy, distribute and transmit the work           #
#  * to Remix - to adapt the work                                   #
# Under the following conditions:                                   #
#  * Attribution - You must attribute the work in the manner        #
#    specified by the author or licensor (but not in any way that   #
#    suggests that they endorse you or your use of the work).       #
# With the understanding that:                                      #
#  * Waiver - Any of the above conditions can be waived if you get  #
#    permission from the copyright holder.                          #
#  * Public Domain - Where the work or any of its elements is in    #
#    the public domain under applicable law, that status is in no   #
#    way affected by the license.                                   #
#  * Other Rights - In no way are any of the following rights       #
#    affected by the license:                                       #
#     - Your fair dealing or fair use rights, or other applicable   #
#       copyright exceptions and limitations;                       #
#     - The author's moral rights;                                  #
#     - Rights other persons may have either in the work itself or  #
#       in how the work is used, such as publicity or privacy       #
#       rights.                                                     #
#  * Notice - For any reuse or distribution, you must make clear to #
#    others the license terms of this work. The best way to do this #
#    is with a link to this web page.                               #
#                                                                   #
# See http://creativecommons.org/ for details.                      #
# ----------------------------------------------------------------- #

# 1st field: state before transition
# 2nd field: state after transition
# 3rd field: event (input message)
# 4th field: command (output message)
#
# Model
# MODEL_ADD|(model alias)|(model file name)|(x position),(y position),(z position)|(x rotation),(y rotation),(z rotation)|(ON or OFF for cartoon)|(parent model alias)|(parent bone name)
# MODEL_CHANGE|(model alias)|(model file name)
# MODEL_DELETE|(model alias)
# MODEL_EVENT_ADD|(model alias)
# MODEL_EVENT_CHANGE|(model alias)
# MODEL_EVENT_DELETE|(model alias)
#
# Motion
# MOTION_ADD|(model alias)|(motion alias)|(motion file name)|(FULL or PART)|(ONCE or LOOP)|(ON or OFF for smooth)|(ON or OFF for repos)
# MOTION_ACCELERATE|(model alias)|(motion alias)|(speed)|(duration)|(specified time for end)
# MOTION_CHANGE|(model alias)|(motion alias)|(motion file name)
# MOTION_DELETE|(mpdel alias)|(motion alias)
# MOTION_EVENT_ADD|(model alias)|(motion alias)
# MOTION_EVENT_ACCELERATE|(model alias)|(motion alias)
# MOTION_EVENT_CHANGE|(model alias)|(motion alias)
# MOTION_EVENT_DELETE|(model alias)|(motion alias)
#
# Move and Rotate
# MOVE_START|(model alias)|(x position),(y position),(z position)|(GLOBAL or LOCAL position)|(move speed)
# MOVE_STOP|(model alias)
# MOVE_EVENT_START|(model alias)
# MOVE_EVENT_STOP|(model alias)
# TURN_START|(model alias)|(x position),(y position),(z position)|(GLOBAL or LOCAL position)|(rotation speed)
# TURN_STOP|(model alias)
# TURN_EVENT_START|(model alias)
# TURN_EVENT_STOP|(model alias)
# ROTATE_START|(model alias)|(x rotation),(y rotaion),(z rotation)|(GLOBAL or LOCAL rotation)|(rotation speed)
# ROTATE_STOP|(model alias)
# ROTATE_EVENT_START|(model alias)
# ROTATE_EVENT_STOP|(model alias)
#
# Sound
# SOUND_START|(sound alias)|(sound file name)
# SOUND_STOP|(sound alias)
# SOUND_EVENT_START|(sound alias)
# SOUND_EVENT_STOP|(sound alias)
#
# Stage
# STAGE|(stage file name)
# STAGE|(bitmap file name for floor),(bitmap file name for background)
#
# Light
# LIGHTCOLOR|(red),(green),(blue)
# LIGHTDIRECTION|(x position),(y position),(z position)
#
# Camera
# CAMERA|(x position),(y position),(z position)|(x rotation),(y rotation),(z rotation)|(distance)|(fovy)|(time)
# CAMERA|(motion file name)
#
# Speech recognition
# RECOG_EVENT_START
# RECOG_EVENT_STOP|(word sequence)
# RECOG_MODIFY|GAIN|(ratio)
# RECOG_MODIFY|USERDICT_SET|(dictionary file name)
# RECOG_MODIFY|USERDICT_UNSET
#
# Speech synthesis
# SYNTH_START|(model alias)|(voice alias)|(synthesized text)
# SYNTH_STOP|(model alias)
# SYNTH_EVENT_START|(model alias)
# SYNTH_EVENT_STOP|(model alias)
# LIPSYNC_START|(model alias)|(phoneme and millisecond pair sequence)
# LIPSYNC_STOP|(model alias)
# LIPSYNC_EVENT_START|(model alias)
# LIPSYNC_EVENT_STOP|(model alias)
#
# Variable
# VALUE_SET|(variable alias)|(value)
# VALUE_SET|(variable alias)|(minimum value for random)|(maximum value for random)
# VALUE_UNSET|(variable alias)
# VALUE_EVAL|(variable alias)|(EQ or NE or LE or LT or GE or GT for evaluation)|(value)
# VALUE_EVENT_SET|(variable alias)
# VALUE_EVENT_UNSET|(variable alias)
# VALUE_EVENT_EVAL|(variable alias)|(EQ or NE or LE or LT or GE or GT for evaluation)|(value)|(TRUE or FALSE)
# TIMER_START|(count down alias)|(value)
# TIMER_STOP|(count down alias)
# TIMER_EVENT_START|(count down alias)
# TIMER_EVENT_STOP|(count down alias)
#
# Plugin
# PLUGIN_ENABLE|(plugin name)
# PLUGIN_DISABLE|(plugin name)
# PLUGIN_EVENT_ENABLE|(plugin name)
# PLUGIN_EVENT_DISABLE|(plugin name)
#
# Other events
# DRAGANDDROP|(file name)
# KEY|(key name)
#
# Other commands
# EXECUTE|(file name)
# KEY_POST|(window class name)|(key name)|(ON or OFF for shift-key)|(ON or OFF for ctrl-key)|(On or OFF for alt-key)

# 0011-0020 Initialization

0    12   <eps>                                      MODEL_ADD|bootscreen|Accessory\bootscreen\bootscreen.pmd|0.0,12.85,17.6|0.0,0.0,0.0|OFF
#11   12   MODEL_EVENT_ADD|bootscreen                 MODEL_ADD|mei|Model\mei\mei.pmd|0.0,0.0,-14.0
12   13   <eps>                                      MODEL_ADD|menu|Accessory\menu\menu.pmd|0.0,-4.5,0.0|0.0,0.0,0.0|ON|mei
13   14   <eps>                                      MOTION_ADD|menu|rotate|Motion\menu_rotation\menu_rotation.vmd|FULL|LOOP|OFF|OFF
14   15   <eps>                                      STAGE|Stage\building2\floor.bmp,Stage\building2\場面1.png
15   16   <eps>                                      MOTION_ADD|mei|base|Motion\mei_wait\mei_wait.vmd|FULL|LOOP|ON|OFF
16   17   <eps>                                      TIMER_START|bootscreen|1.5
#17   18   <eps>                                     HIKARI_START|アップ
17   1    TIMER_EVENT_STOP|bootscreen                MODEL_DELETE|bootscreen

# 0021-0030 Idle behavior

#2    21   <eps>                                     TIMER_START|idle1|20
#21   22   TIMER_EVENT_START|idle1                   TIMER_START|idle2|40
#22   23   TIMER_EVENT_START|idle2                   TIMER_START|idle3|60
#23   1    TIMER_EVENT_START|idle3                   VALUE_SET|random|0|100
#1    1    RECOG_EVENT_START                         MOTION_ADD|mei|listen|Expression\mei_listen\mei_listen.vmd|PART|ONCE
#1    1    TIMER_EVENT_STOP|idle1                    MOTION_ADD|mei|idle|Motion\mei_idle\mei_idle_boredom.vmd|PART|ONCE
#1    1    TIMER_EVENT_STOP|idle2                    MOTION_ADD|mei|idle|Motion\mei_idle\mei_idle_touch_clothes.vmd|PART|ONCE
#1    2    TIMER_EVENT_STOP|idle3                    MOTION_ADD|mei|idle|Motion\mei_idle\mei_idle_think.vmd|PART|ONCE

# 0031-0050 Hikari Agent動作

#1    31   指示語なし|こんにちは                       SYNTH_START|mei|mei_voice_normal|こんにちは。
#1    31   指示語なし|こんにちわ                       SYNTH_START|mei|mei_voice_normal|こんにちは。
#31   39   <eps>                                       HIKARI_START|アップ

#1    32   指示語なし|かわいい                         SYNTH_START|mei|mei_voice_normal|恥ずかしいです。
#32   39   <eps>                                       HIKARI_START|ピンク

#1    33   指示語なし|単純		                      SYNTH_START|mei|mei_voice_normal|怒りますよ。
#33   39   <eps>                                       HIKARI_START|赤

#1    34   指示語なし|バイバイ                         SYNTH_START|mei|mei_voice_normal|さようなら。
#34   39   <eps>                                       HIKARI_START|終了

#39   1    SYNTH_EVENT_STOP|mei                        <eps>


# 0051-0100 speech recognition

1 1       KEY|p                                        VEHICLE_START|右
1 1       KEY|o                                        VEHICLE_START|直進
1 1       KEY|i                                        VEHICLE_START|左
1 1       KEY|u                                        VEHICLE_START|減速
1 1       KEY|y                                        VEHICLE_START|速度をもどす
1 1       KEY|t                                        VEHICLE_START|止める
1 1       KEY|r                                        VEHICLE_START|IB館

#デモ目的地設定
#1 71      detect|person                               SYNTH_START|mei|mei_voice_normal|こんにちは。中川さん。いつも通り、名古屋ナショナルセンターに行きますか？
1 71       KEY|a                                       SYNTH_START|mei|mei_voice_normal|おはようございます。
71 72      SYNTH_EVENT_STOP|mei                        SYNTH_START|mei|mei_voice_normal|いつも通り、ナショナルイノベーションコンプレックスに行きますか？
72 88      swing|kubihuri                              SYNTH_STOP|mei
72 88      KEY|b                                        SYNTH_STOP|mei
72 73      SYNTH_EVENT_STOP|mei                        <eps>
73 79      RECOG_EVENT_STOP|野依記念館                 VEHICLE_START|IB館
73 74      @RECOG_EVENT_STOP\|.*@                      SYNTH_START|mei|mei_voice_normal|もう一度お願いします。
74 73      SYNTH_EVENT_STOP|mei                        <eps>
79 76      <eps>                                       SYNTH_START|mei|mei_voice_normal|わかりました。野依記念学術交流館ですね。では出発してもいいですか？
76 77      SYNTH_EVENT_STOP|mei                        <eps>
77 1       指示語なし|はい                             VEHICLE_START|出発
77 1       指示語なし|うん                             VEHICLE_START|出発
77 1       KEY|e                                       VEHICLE_START|出発
77 89       nod|nodnod                                 VEHICLE_START|出発
80 79      RECOG_EVENT_STOP|野依記念館                 VEHICLE_START|IB館
80 79      KEY|c		                		       VEHICLE_START|IB館                     
80 81      @RECOG_EVENT_STOP\|.*@                      SYNTH_START|mei|mei_voice_normal|もう一度お願いします。
81 80      SYNTH_EVENT_STOP|mei                        <eps>

88 80      <eps>                                       START_MUSIC
89 1       <eps>                                       START_MUSIC


#73 74      ドライバー|発見                             VAD_OK
#73 75      指示語なし|はい                             VEHICLE_START|名古屋ナショナルセンター
#73 75      指示語なし|うん                             VEHICLE_START|名古屋ナショナルセンター
#73 75      nod|nodnod                                  VEHICLE_START|名古屋ナショナルセンター
#74 79      指示語なし|野依記念館                       VEHICLE_START|IB館
#80 82      ドライバー|発見                             VAD_OK
#81 80      SYNTH_EVENT_STOP|mei                        <eps>
#82 79      指示語なし|野依記念館                       VEHICLE_START|IB館
#75 76      <eps>                                       SYNTH_START|mei|mei_voice_normal|わかりました。ナショナルイノベーションコンプレックスですね。では出発しますね？
#77 78      ドライバー|発見                             VAD_OK

1 51       曲がる|NG                                    SYNTH_START|mei|mei_voice_normal|曲がれません。

1 1      ドライバー|発見                               VAD_OK
#1 51      ドライバー|発見                              SYNTH_START|mei|mei_voice_normal|ドライバーの指示です。
#1 51      ドライバー|NG                                SYNTH_START|mei|mei_voice_normal|ドライバーの指示では、ないです。

1 1       指示語なし|よし,行こう                      VEHICLE_START|速度をもどす
1 1       指示語なし|よし,行って                      VEHICLE_START|速度をもどす
1 1       指示語なし|よし,行く                        VEHICLE_START|速度をもどす
1 1       指示語なし|速度,上げて                      VEHICLE_START|速度をもどす
1 1       指示語なし|速度,上げる                      VEHICLE_START|速度をもどす
1 1       指示語なし|スピード,上げて                  VEHICLE_START|スピードをもどす
1 1       指示語なし|スピード,上げる                  VEHICLE_START|スピードをもどす
1 1       指示語なし|速度,もどす                      VEHICLE_START|速度をもどす
1 1       指示語なし|速度,もどして                    VEHICLE_START|速度をもどす
1 1       指示語なし|スピード,もどす                  VEHICLE_START|スピードをもどす
1 1       指示語なし|スピード,もどして                VEHICLE_START|スピードをもどす

1 1       指示語なし|減速                             VEHICLE_START|減速
#1 1        RECOG_EVENT_STOP|減速                             VEHICLE_START|減速
1 1       指示語なし|速度,落とす                      VEHICLE_START|減速
1 1       指示語なし|スピード,落とす                  VEHICLE_START|スピード減速
1 1       指示語なし|速度,落として                    VEHICLE_START|減速
1 1       指示語なし|スピード,落として                VEHICLE_START|スピード減速
1 1       指示語なし|ゆっくり,走る                    VEHICLE_START|減速
1 1       指示語なし|ゆっくり,走って                  VEHICLE_START|減速
1 1       指示語なし|ゆっくり,行く                    VEHICLE_START|減速
1 1       指示語なし|ゆっくり,行こう                  VEHICLE_START|減速
1 1       指示語なし|ゆっくり,行って                  VEHICLE_START|減速
1 1       指示語なし|ゆっくり,進む                    VEHICLE_START|減速
1 1       指示語なし|ゆっくり,進んで                  VEHICLE_START|減速

1 51       減速|OK                                     SYNTH_START|mei|mei_voice_normal|速度を落とします。
1 51       速度|もどす                                 SYNTH_START|mei|mei_voice_normal|速度をもどします。
1 51       スピード減速|OK                             SYNTH_START|mei|mei_voice_normal|スピードを落とします。
1 51       スピード|もどす                             SYNTH_START|mei|mei_voice_normal|スピードをもどします。

#1 1        指示語なし|寄って                           MAP_START
#1 1        RECOG_EVENT_STOP|寄って                     MAP_START
1 1        建物|寄って                                  MAP_START

#1 1        building|hello                              EYE_STOP|病院
1 1        building|IB                                 EYE_STOP|IB館
1 1        building|hospital                           EYE_STOP|病院

1 51       eye_stop|IB館                               SYNTH_START|mei|mei_voice_normal|IB館に寄り道します。
#1 51       eye_stop|病院                               SYNTH_START|mei|mei_voice_normal|工学部二号館に寄り道します。

#1 1       指示語なし|とめる                            VEHICLE_START|止める
#1 1       指示語なし|とめて                            VEHICLE_START|止める
#1 1       指示語なし|とまって                          VEHICLE_START|止める


1 51       止める|OK                                   SYNTH_START|mei|mei_voice_normal|とまります。

1 1       指示語なし|発進                             VEHICLE_START|発進
1 1       指示語なし|出発                             VEHICLE_START|出発
#1 1        RECOG_EVENT_STOP|発進                             VEHICLE_START|発進

1 51       set_start|発進                              SYNTH_START|mei|mei_voice_normal|発進します。
1 51       set_start|出発                              SYNTH_START|mei|mei_voice_normal|出発します。

#1 1        指示語なし|あれ,何                           MAP_START
#1 1        指示語なし|それ,何                           MAP_START
#1 1        指示語なし|建物,何                           MAP_START
1 1         建物|何                                      MAP_START

#1 51        building_info|hello                        SYNTH_START|mei|mei_voice_normal|こんにちは。
1 51        building_info|IB                           SYNTH_START|mei|mei_voice_normal|あれはIB館です。
1 51        building_info|hospital                     SYNTH_START|mei|mei_voice_normal|あれは工学部二号館です。
1 51        building_info|Library                      SYNTH_START|mei|mei_voice_normal|あれは図書館です。
1 51        building_info|Ltrbld                       SYNTH_START|mei|mei_voice_normal|あれは文系総合館です。
1 51        building_info|Unknown                      SYNTH_START|mei|mei_voice_normal|どの建物か、わかりません。

1 1       指示語なし|右,曲がる                         VEHICLE_START|右
1 1       指示語なし|右,曲がっ                         VEHICLE_START|右
1 1       指示語なし|右,曲がって                       VEHICLE_START|右
1 1       指示語なし|右,行く                           VEHICLE_START|右
1 1       指示語なし|右,行っ                           VEHICLE_START|右
1 1       指示語なし|右折                              VEHICLE_START|右
1 1       指示語なし|交差点,右                         VEHICLE_START|右

#1 1       指示語なし|左,曲がる                         VEHICLE_START|左
#1 1       指示語なし|左,曲がっ                         VEHICLE_START|左
#1 1       指示語なし|左,曲がって                       VEHICLE_START|左
#1 1       指示語なし|左,行く                           VEHICLE_START|左
#1 1       指示語なし|左,行っ                           VEHICLE_START|左
#1 1       指示語なし|左折                              VEHICLE_START|左
#1 1       指示語なし|交差点,左                         VEHICLE_START|左

#1 1       指示語なし|まっすぐ                         VEHICLE_START|直進
#1 1       指示語なし|直進                             VEHICLE_START|直進
#1 1       指示語なし|独身                             VEHICLE_START|直進

#1 1        試し|渋滞                                   VEHICLE_START|渋滞

#1 91       渋滞|LEFT_OK                                SYNTH_START|mei|mei_voice_normal|この先、渋滞です。左に迂回してもいいですか？
#91 92      SYNTH_EVENT_STOP|mei                        <eps>
#92 1       指示語なし|はい                             VEHICLE_START|左
#92 1       nod|nodnod                                  VEHICLE_START|左
#92 51      指示語なし|いいえ                           SYNTH_START|mei|mei_voice_normal|わかりました。進路変更しません。
#92 51      swing|kubihuri                              SYNTH_START|mei|mei_voice_normal|わかりました。進路変更しません。

#1 93       渋滞|RIGHT_OK                               SYNTH_START|mei|mei_voice_normal|この先、渋滞です。右に迂回してもいいですか？
#93 94      SYNTH_EVENT_STOP|mei                        <eps>
#94 1       指示語なし|はい                            VEHICLE_START|右
#94 1       nod|nodnod                                 VEHICLE_START|右
#94 51      指示語なし|いいえ                           SYNTH_START|mei|mei_voice_normal|わかりました。進路変更しません。
#94 51      swing|kubihuri                              SYNTH_START|mei|mei_voice_normal|わかりました。進路変更しません。

#1 95       渋滞|FORWARD_OK                             SYNTH_START|mei|mei_voice_normal|この先、渋滞です。直進して迂回してもいいですか？
#95 96      SYNTH_EVENT_STOP|mei                        <eps>
#96 1       指示語なし|はい                             VEHICLE_START|直進
#96 1       nod|nodnod                                  VEHICLE_START|直進
#96 51      指示語なし|いいえ                           SYNTH_START|mei|mei_voice_normal|わかりました。進路変更しません。
#96 51      swing|kubihuri                              SYNTH_START|mei|mei_voice_normal|わかりました。進路変更しません。

#1 51       渋滞|NO                                     SYNTH_START|mei|mei_voice_normal|この先、渋滞ですが、ここでは迂回できません。

1 1       指示語なし2|左                              VEHICLE_START|左
1 1       指示語なし2|右                              VEHICLE_START|右

1 51      指示語なし|ファミリーマート,曲がっ          SYNTH_START|mei|mei_voice_normal|ファミリーマートの手前の道を曲がります。
1 51      指示語なし|ファミリーマート,曲がる          SYNTH_START|mei|mei_voice_normal|ファミリーマートの手前の道を曲がります。
1 51      指示語なし|ファミリーマート,左              SYNTH_START|mei|mei_voice_normal|ファミリーマートの手前の道を曲がります。
#1 51     指示語なし|ファミリーマート,手前,曲がる     SYNTH_START|mei|mei_voice_normal|ファミリーマートの手前の道を曲がります。

1 51      指示語なし|ガソリンスタンド,曲がる          SYNTH_START|mei|mei_voice_normal|ガソリンスタンドの手前の道を曲がります。
1 51      指示語なし|ガソリンスタンド,曲がっ          SYNTH_START|mei|mei_voice_normal|ガソリンスタンドの手前の道を曲がります。
1 51      指示語なし|ガソリンスタンド,右              SYNTH_START|mei|mei_voice_normal|ガソリンスタンドの手前の道を曲がります。
#1 51     指示語なし|ガソリンスタンド,手前,曲がる     SYNTH_START|mei|mei_voice_normal|ガソリンスタンドの手前の道を曲がります。

#1 51     指示語なし2|ファミリーマート                SYNTH_START|mei|mei_voice_normal|ファミリーマートの手前の道を曲がります。
#1 51     指示語なし2|ガソリンスタンド                SYNTH_START|mei|mei_voice_normal|ガソリンスタンドの手前の道を曲がります。

1 51      指示語なし3|ファミリーマート                SYNTH_START|mei|mei_voice_normal|ファミリーマートに車を停めます。
1 51      指示語なし3|ガソリンスタンド                SYNTH_START|mei|mei_voice_normal|ガソリンスタンドに車を停めます。


1 51      VEHICLE_RECEIVE|RIGHT_TURN_ACTION           SYNTH_START|mei|mei_voice_normal|右に曲がります。
1 51      VEHICLE_RECEIVE|LEFT_TURN_ACTION            SYNTH_START|mei|mei_voice_normal|左に曲がります。
1 51      VEHICLE_RECEIVE|FORWARD_ACTION              SYNTH_START|mei|mei_voice_normal|直進します。

1 51      VEHICLE_RECEIVE|SLOWDOWN_ACTION              SYNTH_START|mei|mei_voice_normal|減速します。
#1 51      VEHICLE_RECEIVE|SPEEDUP_ACTION              SYNTH_START|mei|mei_voice_normal|スピードを上げます。
1 51      VEHICLE_RECEIVE|START_ACTION                SYNTH_START|mei|mei_voice_normal|発進します。
1 51      VEHICLE_RECEIVE|STOP_ACTION                 SYNTH_START|mei|mei_voice_normal|車を止めます。

#1 51      ENABLE_VEHICLE_ACTION|RIGHT_TURN            SYNTH_START|mei|mei_voice_normal|この先、右に曲がれます。

#バージイン
#51 60     RECOG_EVENT_START                           SYNTH_STOP|mei
#60 51     SYNTH_EVENT_STOP|mei                        SYNTH_START|mei|mei_voice_normal|はい？


51 1      SYNTH_EVENT_STOP|mei                        <eps>


#0101-0200 LEAPMOTION

1 1       LEAP_EVENT|Lawson                           VEHICLE_START|左
1 1       LEAP_EVENT|GasStation                       VEHICLE_START|右
1 301     LEAP_EVENT|NotApplicable                    SYNTH_START|mei|mei_voice_normal|指差している方向が分かりません。

#1 199     LEAP_EVENT|Lawson                          SYNTH_START|mei|mei_voice_normal|ファミリーマートの手前の道を曲がります。
#1 199     LEAP_EVENT|GasStation                      SYNTH_START|mei|mei_voice_normal|ガソリンスタンドの手前の道を曲がります。
#1 301     LEAP_EVENT|NotApplicable                   SYNTH_START|mei|mei_voice_normal|指差している方向が分かりません。

1 1        LEAP_EVENT2|Lawson                         VEHICLE_START|左
1 1        LEAP_EVENT2|GasStation                     VEHICLE_START|右
1 310      LEAP_EVENT2|NotApplicable                  SYNTH_START|mei|mei_voice_normal|指差している方向が分かりません。

#1 199      LEAP_EVENT2|Lawson                         SYNTH_START|mei|mei_voice_normal|ファミリーマートに車を停めます。
#1 199      LEAP_EVENT2|GasStation                     SYNTH_START|mei|mei_voice_normal|ガソリンスタンドに車を停めます。
#1 310      LEAP_EVENT2|NotApplicable                  SYNTH_START|mei|mei_voice_normal|指差している方向が分かりません。

#バージイン
#399 110    LEAP_EVENT|circle                          SYNTH_STOP|mei
#110 199    SYNTH_EVENT_STOP|mei                       SYNTH_START|mei|mei_voice_normal|なんですか。

1  199     LEAP_EVENT|keytap                          SYNTH_START|mei|mei_voice_normal|車を停めます。

#タイマーイベント
#1  151      state=|3                                   TIMER_START|state_timer|5
#151 199   <eps>                                      SYNTH_START|mei|mei_voice_normal|タイマー。
#1  150    TIMER_EVENT_STOP|state_timer               state_flag
#150 199   <eps>                                      SYNTH_START|mei|mei_voice_normal|初期状態にもどります。

199 1      SYNTH_EVENT_STOP|mei                       <eps>



#0201-0300 eye-gaze

1   1      sisen|left                                 HIKARI_START|アップ

201 210    SYNTH_EVENT_STOP|mei                       <eps>
210 1      nod|nodnod                                 VEHICLE_START|右
210 1      指示語なし|はい                            VEHICLE_START|右
210 1      指示語なし|曲がる                          VEHICLE_START|右
210 300    swing|kubihuri                             SYNTH_START|mei|mei_voice_normal|曲がりません。
210 300    指示語なし|いいえ                          SYNTH_START|mei|mei_voice_normal|曲がりません。

300 1      SYNTH_EVENT_STOP|mei                        <eps>



#0301-0400 Speech + LEAPMOTION

1 1       leap+julius|That=Lawson,Turn                VEHICLE_START|左
1 201     leap+julius|That=GasStation,Turn            SYNTH_START|mei|mei_voice_normal|右に曲がりますか？
1 301     leap+julius|That=NotApplicable              SYNTH_START|mei|mei_voice_normal|もう一度指さしをしてください。

1 1       leap+julius|This=Lawson,Turn                VEHICLE_START|右
1 1       leap+julius|This=GasStation,Turn            VEHICLE_START|左
1 301     leap+julius|This=NotApplicable              SYNTH_START|mei|mei_voice_normal|もう一度指さしをしてください。

#1 399     leap+julius|That=Lawson,Turn               SYNTH_START|mei|mei_voice_normal|ファミリーマートの手前の道を曲がります。
#1 399     leap+julius|That=GasStation,Turn           SYNTH_START|mei|mei_voice_normal|ガソリンスタンドの手前の道を曲がります。
#1 301     leap+julius|That=NotApplicable             SYNTH_START|mei|mei_voice_normal|もう一度指さしをしてください。

#1 399     leap+julius|This=Lawson,Turn               SYNTH_START|mei|mei_voice_normal|ファミリーマートの手前の道を曲がります。
#1 399     leap+julius|This=GasStation,Turn           SYNTH_START|mei|mei_voice_normal|ガソリンスタンドの手前の道を曲がります。
#1 301     leap+julius|This=NotApplicable             SYNTH_START|mei|mei_voice_normal|もう一度指さしをしてください。

1 399      leap+julius|That=Lawson,Stop               SYNTH_START|mei|mei_voice_normal|ファミリーマートに車を停めます。
1 399      leap+julius|That=GasStation,Stop           SYNTH_START|mei|mei_voice_normal|ガソリンスタンドに車を停めます。
1 310      leap+julius|That=NotApplicable             SYNTH_START|mei|mei_voice_normal|もう一度指さしをしてください。

1 399      leap+julius|This=Lawson,Stop               SYNTH_START|mei|mei_voice_normal|ファミリーマートに車を停めます。
1 399      leap+julius|This=GasStation,Stop           SYNTH_START|mei|mei_voice_normal|ガソリンスタンドに車を停めます。
1 310      leap+julius|This=NotApplicable             SYNTH_START|mei|mei_voice_normal|もう一度指さしをしてください。

#1 399     leap+julius|あそこ=ファミリーマート        SYNTH_START|mei|mei_voice_normal|ファミリーマートの手前の曲がります。
#1 399     leap+julius|あそこ=ガソリンスタンド        SYNTH_START|mei|mei_voice_normal|ガソリンスタンドの手前の道を曲がります。
#1 301     leap+julius|あそこ=該当なし                SYNTH_START|mei|mei_voice_normal|もう一度指さしをしてください。

1   301    leap+julius|NotPoint,Turn                  SYNTH_START|mei|mei_voice_normal|どこですか。
301 302    SYNTH_EVENT_STOP|mei                       TIMER_START|mati1|0.4
302 1      TIMER_EVENT_STOP|mati1                     yubi_flag

1   310    leap+julius|NotPoint,Stop                  SYNTH_START|mei|mei_voice_normal|どこですか。
310 311    SYNTH_EVENT_STOP|mei                       TIMER_START|mati2|0.4
311 1      TIMER_EVENT_STOP|mati2                     yubi_flag

399 1      SYNTH_EVENT_STOP|mei                       <eps>

