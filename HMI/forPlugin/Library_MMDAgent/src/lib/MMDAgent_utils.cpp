/* ----------------------------------------------------------------- */
/*           The Toolkit for Building Voice Interaction Systems      */
/*           "MMDAgent" developed by MMDAgent Project Team           */
/*           http://www.mmdagent.jp/                                 */
/* ----------------------------------------------------------------- */
/*                                                                   */
/*  Copyright (c) 2009-2016  Nagoya Institute of Technology          */
/*                           Department of Computer Science          */
/*                                                                   */
/* All rights reserved.                                              */
/*                                                                   */
/* Redistribution and use in source and binary forms, with or        */
/* without modification, are permitted provided that the following   */
/* conditions are met:                                               */
/*                                                                   */
/* - Redistributions of source code must retain the above copyright  */
/*   notice, this list of conditions and the following disclaimer.   */
/* - Redistributions in binary form must reproduce the above         */
/*   copyright notice, this list of conditions and the following     */
/*   disclaimer in the documentation and/or other materials provided */
/*   with the distribution.                                          */
/* - Neither the name of the MMDAgent project team nor the names of  */
/*   its contributors may be used to endorse or promote products     */
/*   derived from this software without specific prior written       */
/*   permission.                                                     */
/*                                                                   */
/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND            */
/* CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,       */
/* INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF          */
/* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE          */
/* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS */
/* BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,          */
/* EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED   */
/* TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,     */
/* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON */
/* ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,   */
/* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY    */
/* OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE           */
/* POSSIBILITY OF SUCH DAMAGE.                                       */
/* ----------------------------------------------------------------- */

/* headers */
#ifdef __APPLE__
#include <CoreFoundation/CoreFoundation.h>
#endif /* __APPLE__ */

#include <sys/types.h>
#include <sys/stat.h>

#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#include <dlfcn.h>
#include <dirent.h>
#endif /* !_WIN32 */

#include "MMDAgent.h"

/* MMDAgent_getcharsize: get character size */
unsigned char MMDAgent_getcharsize(const char *str)
{
   return MMDFiles_getcharsize(str);
}

/* MMDAgent_strequal: string matching */
bool MMDAgent_strequal(const char *str1, const char *str2)
{
   return MMDFiles_strequal(str1, str2);
}

/* MMDAgent_strheadmatch: match head string */
bool MMDAgent_strheadmatch(const char *str1, const char *str2)
{
   return MMDFiles_strheadmatch(str1, str2);
}

/* MMDAgent_strtailmatch: match tail string */
bool MMDAgent_strtailmatch(const char *str1, const char *str2)
{
   return MMDFiles_strtailmatch(str1, str2);
}

/* MMDAgent_strlen: strlen */
int MMDAgent_strlen(const char *str)
{
   return MMDFiles_strlen(str);
}

/* MMDAgent_strdup: strdup */
char *MMDAgent_strdup(const char *str)
{
   return MMDFiles_strdup(str);
}

/* MMDAgent_strdup_from_utf8_to_sjis: strdup with conversion from utf8 to sjis */
char *MMDAgent_strdup_from_utf8_to_sjis(const char *str)
{
   return MMDFiles_strdup_from_utf8_to_sjis(str);
}

/* MMDAgent_pathdup_from_application_to_system_locale: convert path charset from application to system locale */
char *MMDAgent_pathdup_from_application_to_system_locale(const char *str)
{
   return MMDFiles_pathdup_from_application_to_system_locale(str);
}

/* MMDAgent_pathdup_from_system_locale_to_application: convert path charset from system locale to application */
char *MMDAgent_pathdup_from_system_locale_to_application(const char *str)
{
   return MMDFiles_pathdup_from_system_locale_to_application(str);
}

/* MMDAgent_intdup: integer type strdup */
char *MMDAgent_intdup(const int digit)
{
   int i, size;
   char *p;

   if(digit == 0) {
      size = 2;
   } else {
      if(digit < 0) {
         size = 2;
         i = -digit;
      } else {
         size = 1;
         i = digit;
      }
      for (; i != 0; size++)
         i /= 10;
   }

   p = (char *) malloc(sizeof(char) * size);
   sprintf(p, "%d", digit);
   return p;
}

/* MMDAgent_dirname: get directory name from path */
char *MMDAgent_dirname(const char *file)
{
   return MMDFiles_dirname(file);
}

/* MMDAgent_basename: get file name from path */
char *MMDAgent_basename(const char *file)
{
   return MMDFiles_basename(file);
}

/* MMDAgent_fopen: get file pointer */
FILE *MMDAgent_fopen(const char *file, const char *mode)
{
   return MMDFiles_fopen(file, mode);
}

/* MMDAgent_strtok: strtok */
char *MMDAgent_strtok(char *str, const char *pat, char **save)
{
   char *s = NULL, *e = NULL, *p;
   const char *q;
   char mbc1[MMDFILESUTILS_MAXCHARBYTE + 1];
   char mbc2[MMDFILESUTILS_MAXCHARBYTE + 1];
   int find;
   int step = 0;
   unsigned char i, size;

   if(str != NULL)
      p = str;
   else if(save != NULL)
      p = *save;
   else
      return NULL;
   while(*p != '\0') {
      if(step == 0)
         s = p;
      if(step == 1)
         e = p;
      size = MMDAgent_getcharsize(p);
      for(i = 0; i < size; i++) {
         mbc1[i] = *p;
         if(*p == '\0') {
            i = 0;
            break;
         }
         p++;
      }
      mbc1[i] = '\0';
      /* search */
      find = 0;
      q = pat;
      while(*q != '\0') {
         size = MMDAgent_getcharsize(q);
         for(i = 0; i < size; i++) {
            mbc2[i] = *q;
            if(*q == '\0') {
               i = 0;
               break;
            }
            q++;
         }
         mbc2[i] = '\0';
         if(strcmp(mbc1, mbc2) == 0) {
            find = 1;
            break;
         }
      }
      /* check */
      if(step == 0) {
         if(find == 0)
            step = 1;
      } else {
         if(find == 1) {
            *e = '\0';
            *save = p;
            return s;
         }
      }
   }

   if(step == 1) {
      *save = p;
      return s;
   }

   *save = p;
   return NULL;
}

/* MMDAgent_str2bool: convert string to boolean */
bool MMDAgent_str2bool(const char *str)
{
   if(str == NULL)
      return false;
   else if(strcmp(str, "true") == 0)
      return true;
   else
      return false;
}

/* MMDAgent_str2int: convert string to integer */
int MMDAgent_str2int(const char *str)
{
   if(str == NULL)
      return 0;
   return atoi(str);
}

/* MMDAgent_str2float: convert string to float */
float MMDAgent_str2float(const char *str)
{
   if(str == NULL)
      return 0.0f;
   return (float) atof(str);
}

/* MMDAgent_str2double: convert string to double */
double MMDAgent_str2double(const char *str)
{
   if(str == NULL)
      return 0.0;
   return atof(str);
}

/* MMDAgent_str2ivec: convert string to integer vector */
bool MMDAgent_str2ivec(const char *str, int *vec, const int size)
{
   int i = 0;
   char *buff, *p, *save = NULL;

   if(str == NULL)
      return false;
   buff = MMDAgent_strdup(str);
   for(p = MMDAgent_strtok(buff, ",", &save); p && i < size; p = MMDAgent_strtok(NULL, ",", &save))
      vec[i++] = atoi(p);
   free(buff);
   if(i == size)
      return true;
   else
      return false;
}

/* MMDAgent_str2fvec: convert string to float vector */
bool MMDAgent_str2fvec(const char *str, float *vec, const int size)
{
   int i = 0;
   char *buff, *p, *save = NULL;

   if(str == NULL)
      return false;
   buff = MMDAgent_strdup(str);
   for(p = MMDAgent_strtok(buff, ",", &save); p && i < size; p = MMDAgent_strtok(NULL, ",", &save))
      vec[i++] = (float) atof(p);
   free(buff);
   if(i == size)
      return true;
   else
      return false;
}

/* MMDAgent_str2pos: get position from string */
bool MMDAgent_str2pos(const char *str, btVector3 *pos)
{
   float vec[3];

   if (MMDAgent_str2fvec(str, vec, 3) == false)
      return false;

   pos->setValue(btScalar(vec[0]), btScalar(vec[1]), btScalar(vec[2]));

   return true;
}

/* MMDAgent_str2rot: get rotation from string */
bool MMDAgent_str2rot(const char *str, btQuaternion *rot)
{
   float vec[3];

   if (MMDAgent_str2fvec(str, vec, 3) == false)
      return false;

   rot->setEulerZYX(btScalar(MMDFILES_RAD(vec[2])), btScalar(MMDFILES_RAD(vec[1])), btScalar(MMDFILES_RAD(vec[0])));

   return true;
}

/* MMDAgent_fgettoken: get token from file pointer */
int MMDAgent_fgettoken(FILE *fp, char *buff)
{
   int i;
   char c;

   c = fgetc(fp);
   if(c == EOF) {
      buff[0] = '\0';
      return 0;
   }

   if(c == '#') {
      for(c = fgetc(fp); c != EOF; c = fgetc(fp))
         if(c == '\n')
            return MMDAgent_fgettoken(fp, buff);
      buff[0] = '\0';
      return 0;
   }

   if(c == ' ' || c == '\t' || c == '\r' || c == '\n')
      return MMDAgent_fgettoken(fp, buff);

   buff[0] = c;
   for(i = 1, c = fgetc(fp); c != EOF && c != '#' && c != ' ' && c != '\t' && c != '\r' && c != '\n'; c = fgetc(fp))
      buff[i++] = c;
   buff[i] = '\0';

   if(c == '#')
      fseek(fp, -1, SEEK_CUR);
   if(c == EOF)
      fseek(fp, 0, SEEK_END);

   return i;
}

/* MMDAgent_pwddup: get current directory */
char *MMDAgent_pwddup()
{
   char buff[MMDAGENT_MAXBUFLEN];
   bool result;
   char *path;

#ifdef _WIN32
   result = (GetCurrentDirectoryA(MMDAGENT_MAXBUFLEN, buff) != 0) ? true : false;
#else
   result = (getcwd(buff, MMDAGENT_MAXBUFLEN) != NULL) ? true : false;
#endif /* _WIN32 */
   if(result == false)
      return NULL;

   path = MMDFiles_pathdup_from_system_locale_to_application(buff);
   if(path == NULL)
      return NULL;

   return path;
}

/* MMDAgent_chdir: change current directory */
bool MMDAgent_chdir(const char *dir)
{
   bool result;
   char *path;

   path = MMDAgent_pathdup_from_application_to_system_locale(dir);
   if(path == NULL)
      return false;

#ifdef _WIN32
   result = SetCurrentDirectoryA(path) != 0 ? true : false;
#else
   result = chdir(path) == 0 ? true : false;
#endif /* _WIN32 */
   free(path);

   return result;
}

/* MMDAgent_sleep: sleep in sec */
void MMDAgent_sleep(double t)
{
   glfwSleep(t);
}

/* MMDAgent_setTime: set time in sec */
void MMDAgent_setTime(double t)
{
   glfwSetTime(t);
}

/* MMDAgent_getTime: get time in sec */
double MMDAgent_getTime()
{
   return glfwGetTime();
}

/* MMDAgent_diffTime: get difference between two times in sec */
double MMDAgent_diffTime(double now, double past)
{
   if (past > now)
      return past - now; /* timer overflow is not taken into account */
   else
      return now - past;
}

/* MMDAgent_dlopen: open dynamic library */
void *MMDAgent_dlopen(const char *file)
{
   char *path;
   void *d;

   if(file == NULL)
      return NULL;

   path = MMDAgent_pathdup_from_application_to_system_locale(file);
   if(path == NULL)
      return NULL;

#ifdef _WIN32
   d = (void *) LoadLibraryExA(path, NULL, 0);
#else
   d = dlopen(path, RTLD_NOW);
#endif /* _WIN32 */

   free(path);
   return d;
}

/* MMDAgent_dlclose: close dynamic library */
void MMDAgent_dlclose(void *handle)
{
#ifdef _WIN32
   FreeLibrary((HMODULE) handle);
#else
   dlclose(handle);
#endif /* _WIN32 */
}

/* MMDAgent_dlsym: get function from dynamic library */
void *MMDAgent_dlsym(void *handle, const char *name)
{
#ifdef _WIN32
   return (void *) GetProcAddress((HMODULE) handle, name);
#else
   return dlsym(handle, name);
#endif /* _WIN32 */
}

/* MMDAgent_opendir: open directory */
DIRECTORY *MMDAgent_opendir(const char *name)
{
#ifdef _WIN32
   DIRECTORY *dir;
   char buff[MMDAGENT_MAXBUFLEN];
   char *path;

   if(name == NULL)
      return NULL;

   if(MMDAgent_strlen(name) <= 0)
      strcpy(buff, "*");
   else
      sprintf(buff, "%s%c*", name, MMDAGENT_DIRSEPARATOR);

   path = MMDAgent_pathdup_from_application_to_system_locale(buff);
   if(path == NULL)
      return NULL;

   dir = (DIRECTORY *) malloc(sizeof(DIRECTORY));
   dir->data = malloc(sizeof(WIN32_FIND_DATAA));
   dir->find = FindFirstFileA(path, (WIN32_FIND_DATAA *) dir->data);
   dir->first = true;
   free(path);
   if(dir->find == INVALID_HANDLE_VALUE) {
      free(dir->data);
      free(dir);
      return NULL;
   }
#else
   DIRECTORY *dir;
   char *path;

   if(name == NULL)
      return NULL;

   dir = (DIRECTORY *) malloc(sizeof(DIRECTORY));

   path = MMDFiles_pathdup_from_application_to_system_locale(name);
   if(path == NULL)
      return NULL;
   dir->find = (void *) opendir(path);
   free(path);
   if(dir->find == NULL) {
      free(dir);
      return NULL;
   }
#endif /* _WIN32 */

   return dir;
}

/* MMDAgent_closedir: close directory */
void MMDAgent_closedir(DIRECTORY *dir)
{
   if(dir == NULL)
      return;

#ifdef _WIN32
   FindClose(dir->find);
   free(dir->data);
#else
   closedir((DIR *) dir->find);
#endif /* _WIN32 */
   free(dir);
}

/* MMDAgent_readdir: find files in directory */
bool MMDAgent_readdir(DIRECTORY *dir, char *name)
{
#ifdef _WIN32
   WIN32_FIND_DATAA *dp;
#else
   struct dirent *dp;
#endif /* _WIN32 */

   if(dir == NULL || name == NULL) {
      if(name)
         strcpy(name, "");
      return false;
   }

#ifdef _WIN32
   if(dir->first == true) {
      char *buff;
      dir->first = false;
      dp = (WIN32_FIND_DATAA *) dir->data;
      buff = MMDFiles_pathdup_from_system_locale_to_application(dp->cFileName); /* if no file, does it work well? */
      strcpy(name, buff);
      free(buff);
      return true;
   } else if(FindNextFileA(dir->find, (WIN32_FIND_DATAA *) dir->data) == 0) {
      strcpy(name, "");
      return false;
   } else {
      char *buff;
      dp = (WIN32_FIND_DATAA *) dir->data;
      buff = MMDFiles_pathdup_from_system_locale_to_application(dp->cFileName);
      strcpy(name, buff);
      free(buff);
      return true;
   }
#else
   dp = readdir((DIR *) dir->find);
   if(dp == NULL) {
      strcpy(name, "");
      return false;
   } else {
      char *buff;
      buff = MMDFiles_pathdup_from_system_locale_to_application(dp->d_name);
      strcpy(name, buff);
      free(buff);
      return true;
   }
#endif /* _WIN32 */
}

/* MMDAgent_stat: get file attributes */
MMDAGENT_STAT MMDAgent_stat(const char *file)
{
   char *path;
   MMDAGENT_STAT ret;

   if (file == NULL)
      return MMDAGENT_STAT_UNKNOWN;

   path = MMDAgent_pathdup_from_application_to_system_locale(file);
   if (path == NULL)
      return MMDAGENT_STAT_UNKNOWN;

   struct stat st;
   if (stat(path, &st) == -1) {
      ret = MMDAGENT_STAT_UNKNOWN;
   } else if ((st.st_mode & S_IFMT) == S_IFDIR) {
      ret = MMDAGENT_STAT_DIRECTORY;
#ifndef _WIN32
   } else if ((st.st_mode & S_IFMT) == S_IFLNK) {
      ret = MMDAGENT_STAT_NORMAL;
#endif /* !_WIN32 */
   } else if ((st.st_mode & S_IFMT) == S_IFREG) {
      ret = MMDAGENT_STAT_NORMAL;
   } else {
      ret = MMDAGENT_STAT_UNKNOWN;
   }

   free(path);
   return ret;
}

/* MMDAgent_roundf: round value */
float MMDAgent_roundf(float f)
{
   return (f >= 0.0f) ? floor(f + 0.5f) : ceil(f - 0.5f);
}

/* MMDAgent_mkdir: make directory */
bool MMDAgent_mkdir(const char *name)
{
   char *path;

   path = MMDFiles_pathdup_from_application_to_system_locale(name);
   if(path == NULL)
      return false;

#ifdef _WIN32
   if(!CreateDirectoryA(path, NULL)) {
      free(path);
      return false;
   }
#else
   if(mkdir(path, S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | S_IXGRP | S_IROTH | S_IXOTH | S_IXOTH) != 0) {
      free(path);
      return false;
   }
#endif /* _WIN32 */
   free(path);

   return true;
}

/* MMDAgent_rmdir: remove directory */
bool MMDAgent_rmdir(const char *name)
{
   char buff1[MMDAGENT_MAXBUFLEN];
   char buff2[MMDAGENT_MAXBUFLEN];
   char *p;
   DIRECTORY *dir;

   dir = MMDAgent_opendir(name);
   if(dir != NULL) {
      while(MMDAgent_readdir(dir, buff1) == true) {
         if(MMDAgent_strequal(buff1, ".") == true || MMDAgent_strequal(buff1, "..") == true)
            continue;
         sprintf(buff2, "%s%c%s", name, MMDAGENT_DIRSEPARATOR, buff1);
         MMDAgent_rmdir(buff2);
         p = MMDFiles_pathdup_from_application_to_system_locale(buff2);
#ifdef _WIN32
         DeleteFileA(p);
         RemoveDirectoryA(p);
#else
         remove(p);
         rmdir(p);
#endif /* _WIN32 */
         free(p);
      }
      MMDAgent_closedir(dir);
      p = MMDFiles_pathdup_from_application_to_system_locale(name);
#ifdef _WIN32
      RemoveDirectoryA(p);
#else
      rmdir(p);
#endif /* _WIN32 */
      free(p);
   }
   return true;
}

/* MMDAgent_tmpdirdup: duplicate temporary directory */
char *MMDAgent_tmpdirdup()
{
   char *path;
   char buff1[MMDAGENT_MAXBUFLEN];

#if defined(_WIN32)
   char buff2[MMDAGENT_MAXBUFLEN];
   if(GetTempPathA(MMDAGENT_MAXBUFLEN, buff2) == 0)
      return NULL;
   sprintf(buff1, "%s%s%d", buff2, "MMDAgent-", (int) GetCurrentProcessId());
#elif defined(__ANDROID__)
   sprintf(buff1, "%s%s%d%s%d", "/sdcard/", "MMDAgent-", getuid(), "-", getpid());
#elif defined(__APPLE__)
#if TARGET_OS_IPHONE
   CFStringRef cfs;
   size_t size;
   cfs = CFUUIDCreateString(NULL, CFUUIDCreate(NULL));
   size = CFStringGetMaximumSizeForEncoding(CFStringGetLength(cfs), kCFStringEncodingUTF8) + 1;
   path = (char *) malloc(size);
   CFStringGetCString(cfs, path, size, kCFStringEncodingUTF8);
   CFRelease(cfs);
   sprintf(buff1, "%s/tmp/%s%s", getenv("HOME"), "MMDAgent-", path);
   free(path);
#else /* ~TARGET_OS_IPHONE */
   sprintf(buff1, "%s%s%d%s%d", getenv("TMPDIR"), "MMDAgent-", getuid(), "-", getpid());
#endif /* TARGET_OS_IPHONE */
#else
   sprintf(buff1, "%s%s%d%s%d", "/tmp/", "MMDAgent-", getuid(), "-", getpid());
#endif /* _WIN32 && __ANDROID__ */

   path = MMDFiles_pathdup_from_system_locale_to_application(buff1);
   if(path == NULL)
      return NULL;

   return path;
}

/* MMDAgent_setOrtho: set ortho matrix, equivalent to glOrtho() */
void MMDAgent_setOrtho(float left, float right, float bottom, float top, float vnear, float vfar)
{
   float a = 2.0f / (right - left);
   float b = 2.0f / (top - bottom);
   float c = -2.0f / (vfar - vnear);
   float tx = -(right + left) / (right - left);
   float ty = -(top + bottom) / (top - bottom);
   float tz = -(vfar + vnear) / (vfar - vnear);
   float ortho[16] = {
      a, 0, 0, 0,
      0, b, 0, 0,
      0, 0, c, 0,
      tx, ty, tz, 1
   };
   glMultMatrixf(ortho);
}
