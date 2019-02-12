#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list_keyboard_event.h"

int kb_list_init(KBEventNode **headNode)
{

	*headNode = (KBEventNode *)malloc(sizeof(KBEventNode));
	if (*headNode == NULL)
	{
		(*headNode)->next = NULL;
		
		return -1;
	}
	else
	{

		(*headNode)->next = NULL;
		return 0;
	}
}

void kb_list_printf(KBEventNode *headNode)
{
	int i = 0;
	KBEventNode *pMove = headNode->next;
	printf("kb_list_printf:\r\n");
	while (pMove != NULL)
	{
		i++;
		printf("%d\t%d\t%d\r\n", i,pMove->code, pMove->event);
		pMove = pMove->next;
	}
}

int kb_list_clear(KBEventNode *headNode)
{
	KBEventNode *pMove = headNode->next;
	KBEventNode *pNode;
	while (pMove != NULL)
	{
		pNode = pMove->next;
		free(pMove);
		pMove = pNode;
	}
	headNode->next = NULL;
	return 0;
}


int kb_list_get_size(KBEventNode *headNode)
{
	int i = 0;
	KBEventNode *pMove = headNode->next;
	while (pMove != NULL)
	{
		i++;
		pMove = pMove->next;
	}

	return i;
}

int kb_list_copy(KBEventNode *sourceHeadNode,KBEventNode *targetHeadNode)
{

	KBEventNode *pMove = sourceHeadNode->next;
    kb_list_clear(targetHeadNode);

    
	while (pMove != NULL)
	{
		pMove = pMove->next;

        kb_list_insert(targetHeadNode,pMove->code,pMove->event);
	}

	return 0;
}

KBEventNode* kb_list_get_node(KBEventNode *headNode,char code)
{

	KBEventNode *pMove = headNode->next;

	while (pMove != NULL)
	{
		if (pMove->code == code)
		{
			return pMove;
		}
		pMove = pMove->next;
	}

	return NULL;
}

int kb_list_modify_node(KBEventNode *headNode,char code,char event)
{

	KBEventNode *pMove = headNode->next;

	while (pMove != NULL)
	{
		if (pMove->code == code)
		{
			pMove->event = event;
			return 0;
		}
		pMove = pMove->next;
	}

	return -1;
}

int kb_list_delete_node(KBEventNode *headNode,char code)
{
	KBEventNode *pMoveLast = headNode;
	KBEventNode *pMove = headNode->next;

	while (pMove != NULL)
	{
		if (pMove->code == code)
		{
			pMoveLast->next = pMove->next;
            free(pMove);
			return 0;
		}
		pMoveLast = pMove;
		pMove = pMove->next;
	}
	return -1;
}

int kb_list_insert(KBEventNode *headNode,char code, char event)
{
	KBEventNode *pMoveLast = headNode;
	KBEventNode *pMove = headNode->next;
	KBEventNode *pNode = (KBEventNode *)malloc(sizeof(KBEventNode));
	pNode->code = code;
	pNode->event = event;
	pNode->next = NULL;

	
	if (kb_list_modify_node(headNode,code,event) == 0)
	{
		return 0;
	}

	while (pMove != NULL)
	{
		pMoveLast = pMove;
		pMove = pMove->next;
	}

	pMoveLast->next = pNode;

	return 0;
}