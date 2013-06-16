/**
@file select.c Provides functions for SELECT relational operator
 */

#include "select.h"


/** 
 * @author Renata Mesaros
 * @brief Function that implements SELECT relational operator
 * @param srcTable - original table that is used for selection
 * @param destTable - table that contains the result
 * @param condition - condition for selection
 */
int AK_select(char *srcTable,char *destTable,AK_list *attributes,AK_list *condition){

	///calling the relational operator for filtering according to given condition

	AK_selection(srcTable,destTable,condition);


	///help table for the final result
	char *helptable="help_table";
		
	
	table_addresses *src_addr=(table_addresses *) AK_get_table_addresses(destTable);
	int startAddress = 	src_addr->address_from[0];
	
	AK_block *temp_block = (AK_block *) AK_read_block(startAddress);
	

	AK_list_elem list_attributes;
	AK_header header[MAX_ATTRIBUTES];
 	memset(header, 0, sizeof( AK_header ) * MAX_ATTRIBUTES);

	int new_head=0;
	int head_num[6]={7,7,7,7,7,7};
	int head_counter=0;
	
	///new header for the resulting table
	int head;
	
	
	///going through the header of the table of subscore
	///making a new header for the final result from the selected ones from the subscore
	while(strcmp(temp_block->header[head].att_name,"")!=0){
			
		list_attributes=(AK_list_elem) Ak_First_L(attributes);
		int create=0;
		while(list_attributes!=NULL){
			if(strcmp(temp_block->header[head].att_name,list_attributes->data)==0){
				head_num[head_counter++]=head;  ///the ordinal number of the selected attribute
				create=1;
				break;
			}
			else list_attributes=list_attributes->next;
		}
		if(create){
			memcpy(&header[new_head], &temp_block->header[head], sizeof (temp_block->header[head]));
			new_head++;
		}
		head++;
	}
	AK_initialize_new_segment(helptable, SEGMENT_TYPE_TABLE, header);

	free(temp_block);
	
	
	
	AK_list* row_root = (AK_list *) malloc(sizeof (AK_list));
			
    int i, j, k, l, type, size, address;
    char data[MAX_VARCHAR_LENGTH];
	
int b;	
printf("\n\n\n");
	
	for (i = 0; src_addr->address_from[i] != 0; i++) {

			for (j = src_addr->address_from[i]; j < src_addr->address_to[i]; j++) {
				AK_mem_block *temp = (AK_mem_block *) AK_get_block(j);
				if (temp->block->last_tuple_dict_id == 0)
						break;
	
for (k = 0; k < DATA_BLOCK_SIZE;k+=5) {
	if (temp->block->tuple_dict[k].type == FREE_INT)
						break;
		
			int gl=0;
			
			for(l=0;l<5;l++){
				int write=0;
				///if the attribute number is in the selected list, write it in the resulting table
				while(head_num[b]!=7) {
							if(head_num[b++]==l){write=1;break;}
					}
					if(write==1){
						type = temp->block->tuple_dict[l + k].type;
						size = temp->block->tuple_dict[l + k].size;
						
						address = temp->block->tuple_dict[l + k].address;
						memcpy(data, &(temp->block->data[address]), size);
						data[size] = '\0';
						Ak_Insert_New_Element(type, data, helptable, header[gl++].att_name, row_root);
						
						
					}
					b=0;
				}
				Ak_insert_row(row_root);
				Ak_DeleteAll_L(row_root);
	}
}}
  
  AK_print_table(helptable);

	free(temp_block);
	return EXIT_SUCCESS;

}



/**
 * @author Renata Mesaros
 * @brief Function for testing the implementation
 * @return No return value
 */
void AK_select_test(){
printf("\n\n\n ***** SELECT RELATIONAL OPERATOR ***** \n\n\n");


///list of attributes which will be in the result of selection

AK_list *attributes = (AK_list *) malloc(sizeof (AK_list));
Ak_Init_L(attributes);

///list of elements which represent the condition for selection

AK_list *condition = (AK_list *) malloc(sizeof (AK_list));
Ak_Init_L(condition);



char *num = "2005";

char *srcTable="student";
char *destTable="select_result";

Ak_InsertAtEnd_L(TYPE_ATTRIBS, "firstname", sizeof ("firstname"), attributes);
Ak_InsertAtEnd_L(TYPE_ATTRIBS, "year", sizeof ("year"), attributes);
Ak_InsertAtEnd_L(TYPE_ATTRIBS, "weight", sizeof ("weight"), attributes);

Ak_InsertAtEnd_L(TYPE_ATTRIBS, "year", sizeof ("year"), condition);
Ak_InsertAtEnd_L(TYPE_INT, num, sizeof (int), condition);
Ak_InsertAtEnd_L(TYPE_OPERATOR, "<", sizeof ("<"), condition);

printf("\n SELECT firstname,year,weight FROM student WHERE year<2005;\n\n");

AK_select(srcTable,destTable,attributes,condition);
Ak_DeleteAll_L(attributes);
free(attributes);

Ak_DeleteAll_L(condition);
free(condition);

}