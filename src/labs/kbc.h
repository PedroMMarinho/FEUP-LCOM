#pragma once


#include <lcom/lcf.h>
#include <stdint.h>



int(kbc_issue_command)(uint8_t command, uint8_t *ret_arg); 

int(kbc_insert_command)(uint8_t command);

int(kbc_write_args)(uint8_t *args);

int read_outputBuffer(uint8_t *data); 
