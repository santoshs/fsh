#ifndef _ENV_H_
#define _ENV_H_

#define MAX_ENV_SIZE		256
typedef struct aarray_ {
	char *key;
	char *value;
} aarray_t;

extern char * get_env(char *);

#endif /* _ENV_H_ */
