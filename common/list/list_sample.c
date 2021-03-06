/*
 * list_sample.c
 *
 *  Created on: 2019年10月14日
 *      Author: luozhiyong
 */
#include "list.h"

void list_sample(void)
{
	list_t *list = list_new();  // 初始化链表

	list_node_t *node = list_node_new("my value"); // 创建节点
	node->val; // "my value"

	list_rpush(list, list_node_new("value")); // 从尾部增加节点
	list->tail->val; // "value"

	list_node_t *last = list_rpop(list); // 获取未尾节点

	list_lpush(list, list_node_new("value")); // 从头部增加节点
	list->head->val; // "value"

	list_node_t *node1 = list_find(list, "some value");
	list_at(list, 0);  // first
	list_at(list, 1);  // second
	list_at(list, -1); // last
	list_at(list, -3); // third last


	list_remove(list, node);
	list_destroy(list);


	// 遍历链表例子
	list_t *langs = list_new();

	list_node_t *c = list_rpush(langs, list_node_new("c"));
	list_node_t *js = list_rpush(langs, list_node_new("js"));
	list_node_t *ruby = list_rpush(langs, list_node_new("ruby"));

	//list_node_t *node;
	list_iterator_t *it = list_iterator_new(langs, LIST_HEAD);
	while ((node = list_iterator_next(it))) {
	  puts(node->val);
	}

	list_iterator_destroy(it);
	list_destroy(langs);
}



