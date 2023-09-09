#include "validate.h"

char *config_params[PARAM_NUM][PARAM_OPTIONS] = {
    { "system", "HTTPD", "bool", "", "", "" , "", "", "" },
    { "system", "TELNETD", "bool", "", "", "" , "", "", "" },
    { "system", "SSHD", "bool", "", "", "" , "", "", "" },
    { "system", "FTPD", "bool", "", "", "" , "", "", "" },
    { "system", "BUSYBOX_FTPD", "bool", "", "", "" , "", "", "" },
    { "system", "DISABLE_CLOUD", "bool", "", "", "" , "", "", "" },
    { "system", "REC_WITHOUT_CLOUD", "bool", "", "", "" , "", "", "" },
    { "system", "MQTT", "bool", "", "", "" , "", "", "" },
    { "system", "RTSP", "bool", "", "", "" , "", "", "" },
    { "system", "RTSP_ALT", "bool", "", "", "" , "", "", "" },
    { "system", "RTSP_STREAM", "enum", "high", "low", "both" , "", "", "" },
    { "system", "RTSP_AUDIO", "enum", "no", "pcm", "alaw", "ulaw", "aac", "" },
    { "system", "RTSP_AUDIO_NR_LEVEL", "int", "0", "30", "" , "", "", "" },
    { "system", "SPEAKER_AUDIO", "bool", "", "", "" , "", "", "" },
    { "system", "SNAPSHOT", "bool", "", "", "" , "", "", "" },
    { "system", "SNAPSHOT_VIDEO", "bool", "", "", "" , "", "", "" },
    { "system", "SNAPSHOT_LOW", "bool", "", "", "" , "", "", "" },
    { "system", "TIMELAPSE", "bool", "", "", "" , "", "", "" },
    { "system", "TIMELAPSE_FTP", "bool", "", "", "" , "", "", "" },
    { "system", "TIMELAPSE_DT", "int", "1", "1440", "" , "", "", "" },
    { "system", "TIMELAPSE_VDT", "string", "1", "1440", "" , "", "", "" },
    { "system", "ONVIF", "bool", "", "", "" , "", "", "" },
    { "system", "ONVIF_WSDD", "bool", "", "", "" , "", "", "" },
    { "system", "ONVIF_PROFILE", "enum", "high", "low", "both" , "", "", "" },
    { "system", "ONVIF_WM_SNAPSHOT", "bool", "", "", "" , "", "", "" },
    { "system", "ONVIF_NETIF", "string", "", "", "" , "", "", "" },
    { "system", "TIME_OSD", "bool", "", "", "" , "", "", "" },
    { "system", "NTPD", "bool", "", "", "" , "", "", "" },
    { "system", "NTP_SERVER", "string" "", "", "" , "", "", "" },
    { "system", "PROXYCHAINSNG", "bool", "", "", "" , "", "", "" },
    { "system", "SWAP_FILE", "bool", "", "", "" , "", "", "" },
    { "system", "RTSP_PORT", "int", "1", "65535", "" , "", "", "" },
    { "system", "HTTPD_PORT", "int", "1", "65535", "" , "", "", "" },
    { "system", "USERNAME", "string", "", "", "" , "", "", "" },
    { "system", "PASSWORD", "string", "", "", "" , "", "", "" },
    { "system", "TIMEZONE", "string", "", "", "" , "", "", "" },
    { "system", "FREE_SPACE", "int", "0", "100", "" , "", "", "" },
    { "system", "FTP_UPLOAD", "bool", "", "", "" , "", "", "" },
    { "system", "FTP_HOST", "string", "", "", "" , "", "", "" },
    { "system", "FTP_DIR", "string", "", "", "" , "", "", "" },
    { "system", "FTP_DIR_TREE", "bool", "", "", "" , "", "", "" },
    { "system", "FTP_USERNAME", "string" "", "", "" , "", "", "" },
    { "system", "FTP_PASSWORD", "string" "", "", "" , "", "", "" },
    { "system", "FTP_FILE_DELETE_AFTER_UPLOAD", "bool", "", "", "" , "", "", "" },
    { "system", "SSH_PASSWORD", "string", "", "", "" , "", "", "" },
    { "system", "CRONTAB", "string", "", "", "" , "", "", "" },
    { "system", "DEBUG_LOG", "bool", "", "", "" , "", "", "" },

    { "camera", "SWITCH_ON", "bool", "", "", "" , "", "", "-t" },
    { "camera", "SAVE_VIDEO_ON_MOTION", "bool", "", "", "" , "", "", "-v" },
    { "camera", "MOTION_DETECTION", "bool", "", "", "" , "", "", "-O" },
    { "camera", "SENSITIVITY", "enum", "low", "medium", "high" , "", "", "-s" },
    { "camera", "AI_HUMAN_DETECTION", "bool", "", "", "" , "", "", "-a" },
    { "camera", "AI_VEHICLE_DETECTION", "bool", "", "", "" , "", "", "-E" },
    { "camera", "AI_ANIMAL_DETECTION", "bool", "", "", "" , "", "", "-N" },
    { "camera", "FACE_DETECTION", "bool", "", "", "" , "", "", "-c" },
    { "camera", "BABY_CRYING_DETECT", "bool", "", "", "" , "", "", "-B" },
    { "camera", "LED", "bool", "", "", "" , "", "", "-l" },
    { "camera", "ROTATE", "bool", "", "", "" , "", "", "-r" },
    { "camera", "IR", "bool", "", "", "", "", "", "-i" },
    { "camera", "SOUND_DETECTION", "bool", "", "", "", "", "", "-b" },
    { "camera", "SOUND_SENSITIVITY", "int", "30", "90", "", "", "", "-n" },
    { "camera", "MOTION_TRACKING", "bool", "", "", "", "", "", "-o" },
    { "camera", "CRUISE", "enum", "no", "presets", "360", "", "", "-C" }
};

int validate_param(char *file, char *key, char *value)
{
    int i, j;
    long long_low, long_high, long_value;
    int validate = 0;
    int errno;
    char *endptr;

    for (i = 0; i < PARAM_NUM; i++) {
        if (strcasecmp(key, config_params[i][1]) == 0) {
            if (strcasecmp(file, config_params[i][0]) != 0) {
                validate = 0;
                break;
            }
            if (strcasecmp("bool", config_params[i][2]) == 0) {
                if ((strcmp(value, "yes") == 0) || (strcmp(value, "no") == 0)) {
                    validate = 1;
                }
                if ((strcmp(value, "on") == 0) || (strcmp(value, "off") == 0)) {
                    validate = 1;
                }
            } else if (strcasecmp("int", config_params[i][2]) == 0) {
                // Convert low limit string to int
                errno = 0;    /* To distinguish success/failure after call */
                long_low = strtol(config_params[i][3], &endptr, 10);
                validate = 1;

                /* Check for various possible errors */
                if ((errno == ERANGE && (long_low == LONG_MAX || long_low == LONG_MIN)) || (errno != 0 && long_low == 0)) {
                    validate = 0;
                }
                if (endptr == config_params[i][3]) {
                    validate = 0;
                }

                if (validate == 0) return validate;

                // Convert high limit string to int
                errno = 0;    /* To distinguish success/failure after call */
                long_high = strtol(config_params[i][4], &endptr, 10);
                validate = 1;

                /* Check for various possible errors */
                if ((errno == ERANGE && (long_high == LONG_MAX || long_high == LONG_MIN)) || (errno != 0 && long_high == 0)) {
                    validate = 0;
                }
                if (endptr == config_params[i][4]) {
                    validate = 0;
                }

                if (validate == 0) return validate;

                // Convert value string to int
                errno = 0;    /* To distinguish success/failure after call */
                long_value = strtol(value, &endptr, 10);
                validate = 1;

                /* Check for various possible errors */
                if ((errno == ERANGE && (long_value == LONG_MAX || long_value == LONG_MIN)) || (errno != 0 && long_value == 0)) {
                    validate = 0;
                }
                if (endptr == value) {
                    validate = 0;
                }

                if ((long_value < long_low) || (long_value > long_high)) {
                    validate = 0;
                }

            } else if (strcasecmp("string", config_params[i][2]) == 0) {
                validate = 1;
                if (strlen(value) >= PARAM_SIZE) {
                    validate = 0;
                }
            } else if (strcasecmp("enum", config_params[i][2]) == 0) {
                validate = 0;
                for (j = 3; j < PARAM_OPTIONS - 1; j++) {
                    if (config_params[i][j][0] == '\0') {
                        break;
                    }
                    if (strcmp(value, config_params[i][j]) == 0 ) {
                        validate = 1;
                        break;
                    }
                }
            }
            break;
        }
    }

    return validate;
}

int extract_param(char *param, char *file, char *key, int index)
{
    int i;
    int ret = -1;

    for (i = 0; i < PARAM_NUM; i++) {
        if (strcasecmp(key, config_params[i][1]) == 0) {
            if (strcasecmp(file, config_params[i][0]) != 0) {
                break;
            }
            strcpy(param, config_params[i][index]);
            ret = 0;
            break;
        }
    }

    return ret;
}
