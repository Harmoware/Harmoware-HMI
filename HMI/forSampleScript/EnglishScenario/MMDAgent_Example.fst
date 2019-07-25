﻿# 0011-0020 Initialization ###############################################################
0    12   <eps>                                      MODEL_ADD|bootscreen|Accessory\bootscreen\bootscreen.pmd|0.0,12.85,17.6|0.0,0.0,0.0|OFF
#11   12   MODEL_EVENT_ADD|bootscreen                 MODEL_ADD|mei|Model\mei\mei.pmd|0.0,0.0,-14.0
12   13   <eps>                                      MODEL_ADD|menu|Accessory\menu\menu.pmd|0.0,-4.5,0.0|0.0,0.0,0.0|ON|mei
13   14   <eps>                                      MOTION_ADD|menu|rotate|Motion\menu_rotation\menu_rotation.vmd|FULL|LOOP|OFF|OFF
14   15   <eps>                                      STAGE|Stage\building2\floor.bmp,Stage\building2\場面1.png
15   16   <eps>                                      MOTION_ADD|mei|base|Motion\mei_wait\mei_wait.vmd|FULL|LOOP|ON|OFF
16   17   <eps>                                      TIMER_START|bootscreen|1.5
#17   18   <eps>                                     HIKARI_START|アップ
17   1    TIMER_EVENT_STOP|bootscreen                MODEL_DELETE|bootscreen


# 0051-0100 speech recognition #############################################################
1 1       KEY|p                                        VEHICLE_START|右
1 1       KEY|o                                        VEHICLE_START|直進
1 1       KEY|i                                        VEHICLE_START|左
1 1       KEY|u                                        VEHICLE_START|減速
1 1       KEY|y                                        VEHICLE_START|速度をもどす
1 1       KEY|t                                        VEHICLE_START|止める
1 1       KEY|r                                        VEHICLE_START|IB館

# デモ目的地設定 ######################################################################
1 71       KEY|a                                       "SYNTH_START|mei|slt_voice_normal|Hello, nice to meet you again."
71 72      SYNTH_EVENT_STOP|mei                        "SYNTH_START|mei|slt_voice_normal|As usual, will you go to the National Innovatiion Complex?"

72 88      swing|kubihuri                              SYNTH_STOP|mei
72 88      KEY|b                                        SYNTH_STOP|mei
72 73      SYNTH_EVENT_STOP|mei                        <eps>

73 79      RECOG_EVENT_STOP|Noyori-Conference-Hall      VEHICLE_START|IB館
73 74      @RECOG_EVENT_STOP\|.*@                      "SYNTH_START|mei|slt_voice_normal|Excuse me? Please say again."

74 73      SYNTH_EVENT_STOP|mei                        <eps>

88 80      <eps>                                       START_MUSIC

80 79      RECOG_EVENT_STOP|Noyori-Conference-Hall     VEHICLE_START|IB館
80 79      KEY|c		                		       VEHICLE_START|IB館                     
80 81      @RECOG_EVENT_STOP\|.*@                      "SYNTH_START|mei|slt_voice_normal|Excuse me? Please say again."

81 80      SYNTH_EVENT_STOP|mei                        <eps>

79 76      <eps>                                       "SYNTH_START|mei|slt_voice_normal|OK. Noyori Conference Hall. Shall we go now?"

76 77      SYNTH_EVENT_STOP|mei                        <eps>

77 1       指示語なし|はい                              VEHICLE_START|出発
77 1       KEY|e                                       VEHICLE_START|出発
77 89       nod|nodnod                                 VEHICLE_START|出発

89 1       <eps>                                       START_MUSIC


# 自動車の行動時の発話 ####################################################################
1 51       set_start|出発                              "SYNTH_START|mei|slt_voice_normal|We are starting now."
1 51       set_start|発進                              "SYNTH_START|mei|slt_voice_normal|start moving."


# 音声合成終了待ち ########################################################################
51 1      SYNTH_EVENT_STOP|mei                        <eps>

# ドライバー（の顔）認識開始 #####################################################################
1 1      ドライバー|発見                               VAD_OK


# コマンド操作 ###########################################################################
1 51       曲がる|NG                                 "SYNTH_START|mei|slt_voice_normal|Trun not allowed."
1 51       止める|OK                                 "SYNTH_START|mei|slt_voice_normal|Stop here."
1 51       減速|OK                                    "SYNTH_START|mei|slt_voice_normal|Slow down."
1 51       スピード減速|OK                             "SYNTH_START|mei|slt_voice_normal|Slow down."

1 51       速度|もどす                                 "SYNTH_START|mei|slt_voice_normal|Return the speed."
1 51       スピード|もどす                             "SYNTH_START|mei|slt_voice_normal|Return the speed."

1 1        指示語なし|発進                               VEHICLE_START|発進
1 1        指示語なし|出発                               VEHICLE_START|出発

1 1       指示語なし|speed,up                        VEHICLE_START|速度をもどす
1 1       指示語なし|速度,上げて                      VEHICLE_START|速度をもどす
1 1       指示語なし|速度,上げる                      VEHICLE_START|速度をもどす
1 1       指示語なし|スピード,上げて                  VEHICLE_START|スピードをもどす
1 1       指示語なし|スピード,上げる                  VEHICLE_START|スピードをもどす

1 1       指示語なし|return,the,speed                VEHICLE_START|速度をもどす
1 1       指示語なし|速度,もどす                      VEHICLE_START|速度をもどす
1 1       指示語なし|スピード,もどす                  VEHICLE_START|スピードをもどす
1 1       指示語なし|スピード,もどして                VEHICLE_START|スピードをもどす

1 1       "指示語なし|let's,go"                      VEHICLE_START|速度をもどす
1 1       指示語なし|よし,行こう                      VEHICLE_START|速度をもどす
1 1       指示語なし|よし,行って                      VEHICLE_START|速度をもどす
1 1       指示語なし|よし,行く                        VEHICLE_START|速度をもどす

1 1       指示語なし|slow,down                       VEHICLE_START|減速
1 1       指示語なし|減速                             VEHICLE_START|減速
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



# 建物に対する動き ###############################################################################
1 1        建物|寄って                                  MAP_START
1 1        建物|何                                      MAP_START

1 1        building|IB                                 EYE_STOP|IB館
1 1        building|hospital                           EYE_STOP|病院

1 51       eye_stop|IB館                               "SYNTH_START|mei|slt_voice_normal|I will stop by IB building."


# 建物情報が流れたら、それを発言する ###############################################################
1 51        building_info|IB                           "SYNTH_START|mei|slt_voice_normal|That is IB building."
1 51        building_info|hospital                     "SYNTH_START|mei|slt_voice_normal|That is School of Engineering building 2."
1 51        building_info|Library                      "SYNTH_START|mei|slt_voice_normal|That is library"
1 51        building_info|Ltrbld                       "SYNTH_START|mei|slt_voice_normal|That is Conference Hall."
1 51        building_info|Unknown                      "SYNTH_START|mei|slt_voice_normal|I don't know which building."


# コマンド：車を動かす ############################################################################
1 1       指示語なし|turn,right                       VEHICLE_START|右
1 1       指示語なし|右,曲がって                       VEHICLE_START|右
1 1       指示語なし|右,曲がる                         VEHICLE_START|右
1 1       指示語なし|右,曲がっ                         VEHICLE_START|右
1 1       指示語なし|右,行く                           VEHICLE_START|右
1 1       指示語なし|右,行っ                           VEHICLE_START|右
1 1       指示語なし|右折                              VEHICLE_START|右
1 1       指示語なし|交差点,右                         VEHICLE_START|右


# （指示語なし2）コマンド：車を動かす ##########################################################################
1 1       指示語なし2|left                            VEHICLE_START|左
1 1       指示語なし2|right                           VEHICLE_START|右
1 1       指示語なし2|左                              VEHICLE_START|左
1 1       指示語なし2|右                              VEHICLE_START|右


# 自動車動作の報告 #########################################################################################
1 51      VEHICLE_RECEIVE|RIGHT_TURN_ACTION           "SYNTH_START|mei|slt_voice_normal|Turn to the right."
1 51      VEHICLE_RECEIVE|LEFT_TURN_ACTION            "SYNTH_START|mei|slt_voice_normal|Turn to the left."
1 51      VEHICLE_RECEIVE|FORWARD_ACTION              "SYNTH_START|mei|slt_voice_normal|Go straight."

1 51      VEHICLE_RECEIVE|SLOWDOWN_ACTION             "SYNTH_START|mei|slt_voice_normal|Slow down."
1 51      VEHICLE_RECEIVE|SPEEDUP_ACTION              "SYNTH_START|mei|slt_voice_normal|Speed up."
1 51      VEHICLE_RECEIVE|START_ACTION                "SYNTH_START|mei|slt_voice_normal|Start."
1 51      VEHICLE_RECEIVE|STOP_ACTION                 "SYNTH_START|mei|slt_voice_normal|Stop."








# 交差点での目印的建物に対する、車の行動 #######################################################################
1 51      指示語なし|ファミリーマート,曲がっ          SYNTH_START|mei|slt_voice_normal|ファミリーマートの手前の道を曲がります。
1 51      指示語なし|ファミリーマート,曲がる          SYNTH_START|mei|slt_voice_normal|ファミリーマートの手前の道を曲がります。
1 51      指示語なし|ファミリーマート,左              SYNTH_START|mei|slt_voice_normal|ファミリーマートの手前の道を曲がります。

1 51      指示語なし|ガソリンスタンド,曲がる          SYNTH_START|mei|slt_voice_normal|ガソリンスタンドの手前の道を曲がります。
1 51      指示語なし|ガソリンスタンド,曲がっ          SYNTH_START|mei|slt_voice_normal|ガソリンスタンドの手前の道を曲がります。
1 51      指示語なし|ガソリンスタンド,右              SYNTH_START|mei|slt_voice_normal|ガソリンスタンドの手前の道を曲がります。

1 51      指示語なし3|ファミリーマート                SYNTH_START|mei|slt_voice_normal|ファミリーマートに車を停めます。
1 51      指示語なし3|ガソリンスタンド                SYNTH_START|mei|slt_voice_normal|ガソリンスタンドに車を停めます。



# 0101-0200 LEAPMOTION #####################################################################################
1 1       LEAP_EVENT|Lawson                           VEHICLE_START|左
1 1       LEAP_EVENT|GasStation                       VEHICLE_START|右
1 301     LEAP_EVENT|NotApplicable                    "SYNTH_START|mei|slt_voice_normal|I can not understand the direction of finger pointing."

1 1        LEAP_EVENT2|Lawson                         VEHICLE_START|左
1 1        LEAP_EVENT2|GasStation                     VEHICLE_START|右
1 310      LEAP_EVENT2|NotApplicable                  "SYNTH_START|mei|slt_voice_normal|I can not understand the direction of finger pointing."


# バージイン ####################################################
1  199     LEAP_EVENT|keytap                          "SYNTH_START|mei|slt_voice_normal|I will stop the car."

# タイマーイベント ###############################################
199 1      SYNTH_EVENT_STOP|mei                       <eps>

#0201-0300 eye-gaze #############################################################
1   1      sisen|left                                 HIKARI_START|アップ

# LEAP MOTION ####################################################################
201 210    SYNTH_EVENT_STOP|mei                      <eps>
210 1      nod|nodnod                                VEHICLE_START|右
210 1      指示語なし|はい                            VEHICLE_START|右
210 1      指示語なし|曲がる                          VEHICLE_START|右
210 300    swing|kubihuri                           "SYNTH_START|mei|slt_voice_normal|I will not turn."
210 300    指示語なし|いいえ                          "SYNTH_START|mei|slt_voice_normal|I will not turn."

300 1      SYNTH_EVENT_STOP|mei                        <eps>

#0301-0400 Speech + LEAPMOTION ########################################################
1 1       leap+julius|That=Lawson,Turn                VEHICLE_START|左
1 201     leap+julius|That=GasStation,Turn            "SYNTH_START|mei|slt_voice_normal|Shall we turn to the right?"
1 301     leap+julius|That=NotApplicable              "SYNTH_START|mei|slt_voice_normal|Please pointing with your finger again."

1 1       leap+julius|This=Lawson,Turn                VEHICLE_START|右
1 1       leap+julius|This=GasStation,Turn            VEHICLE_START|左
1 301     leap+julius|This=NotApplicable              "SYNTH_START|mei|slt_voice_normal|Please pointing with your finger again."

1 399      leap+julius|That=Lawson,Stop               SYNTH_START|mei|slt_voice_normal|ファミリーマートに車を停めます。
1 399      leap+julius|That=GasStation,Stop           SYNTH_START|mei|slt_voice_normal|ガソリンスタンドに車を停めます。
1 310      leap+julius|That=NotApplicable             "SYNTH_START|mei|slt_voice_normal|Please pointing with your finger again."

1 399      leap+julius|This=Lawson,Stop               SYNTH_START|mei|slt_voice_normal|ファミリーマートに車を停めます。
1 399      leap+julius|This=GasStation,Stop           SYNTH_START|mei|slt_voice_normal|ガソリンスタンドに車を停めます。
1 310      leap+julius|This=NotApplicable             "SYNTH_START|mei|slt_voice_normal|Please pointing with your finger again."

1   301    leap+julius|NotPoint,Turn                  "SYNTH_START|mei|slt_voice_normal|Where is that?"
301 302    SYNTH_EVENT_STOP|mei                       TIMER_START|mati1|0.4
302 1      TIMER_EVENT_STOP|mati1                     yubi_flag

1   310    leap+julius|NotPoint,Stop                  "SYNTH_START|mei|slt_voice_normal|Where is that?"
310 311    SYNTH_EVENT_STOP|mei                       TIMER_START|mati2|0.4
311 1      TIMER_EVENT_STOP|mati2                     yubi_flag

399 1      SYNTH_EVENT_STOP|mei                       <eps>

