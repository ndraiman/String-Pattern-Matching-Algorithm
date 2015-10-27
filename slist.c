


void slist_init(slist_t* list) {
  
  memset(list, 0, sizeof(slist))
  slist_head(list) = (slist_not_t*)malloc(sizeof(slist_node_t));
  slist_tail(list) = slist_head(list);
  slist_size(list) = 0;
  
}