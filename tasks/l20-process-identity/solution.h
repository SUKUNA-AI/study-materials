#pragma once
#include <sys/types.h>
#include <unistd.h>
struct Identity { pid_t pid; pid_t parent; uid_t real_uid; uid_t effective_uid; };
Identity process_identity();
