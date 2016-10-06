#ifndef _EVENT_H_
#define _EVENT_H_

void
init_event_loop(ev_io *watcher,
	            void (*cb)(struct ev_loop *loop, ev_io *watcher, int revents),
	            int fd);

void
accept_cb(struct ev_loop *loop, ev_io *sock_watcher, int revents);

#endif	
