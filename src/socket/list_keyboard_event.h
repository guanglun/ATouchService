#ifndef __LIST_KEYBOARD_EVENT_H__
#define __LIST_KEYBOARD_EVENT_H__

typedef struct _KBEventNode_
{
	char code;
	char event;
	struct _KBEventNode_ *next;
} KBEventNode;

int kb_list_init(KBEventNode *headNode);
void kb_list_printf(KBEventNode *headNode);
int kb_list_clear(KBEventNode *headNode);
int kb_list_get_size(KBEventNode *headNode);
int kb_list_copy(KBEventNode *sourceHeadNode,KBEventNode *targetHeadNode);
KBEventNode* kb_list_get_node(KBEventNode *headNode,char code);
int kb_list_modify_node(KBEventNode *headNode,char code,char event);
int kb_list_delete_node(KBEventNode *headNode,char code);
int kb_list_insert(KBEventNode *headNode,char code, char event);

#endif
