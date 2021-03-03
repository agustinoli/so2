int open_tcp_server(char *port_str);
int open_udp_server(char * port_str);
int open_tcp_client(char *ip_str,char *port_str);
int write_udp(char *ip_str,char *port_str,char *str);
int write_cmd(int sockfd, void *cmd_ptr);
void read_cmd(int sockfd, void *cmd_ptr);
int read_upd_socket(int sockfd,char *ptr);
int send_file(int fd, char *filename_str);
int receive_file (int sockfd,char *filename_str);
