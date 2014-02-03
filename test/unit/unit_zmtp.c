void test_zmtp_socket_open();
void test_zmtp_bind();
void test_zmtp_connect();

START_TEST (test_zmtp)
{
  test_zmtp_socket_open();
  test_zmtp_bind();
  //test_zmtp_connect();
}
END_TEST

void empty_cb(uint16_t ev, struct zmtp_socket* s)
{

}

void test_zmtp_bind()
{
  struct zmtp_socket * sock;
  int ret;
  uint16_t port_be;
  struct pico_ip4 inaddr_link, netmask;  struct pico_device *dev;

  dev = pico_null_create("dummy");
  pico_string_to_ipv4("10.40.0.2", &inaddr_link.addr);
  netmask.addr = long_be(0xFFFF0000);
  ret = pico_ipv4_link_add(dev, inaddr_link, netmask);
  fail_if(ret < 0, "socket> error adding link");

  sock = zmtp_socket_open(PICO_PROTO_IPV4, PICO_PROTO_TCP, ZMQ_TYPE_PUBLISHER, &empty_cb);
  if (sock == NULL)
  {
    fail_if(1,"zmtp_bind: unable to initiate zmtp_socket");
  } else {
    
    port_be = short_be(5555);
    /* socket_bind passing wrong parameters */
    ret = zmtp_socket_bind(NULL, &inaddr_link, &port_be);
    fail_if(ret == 0, "socket> tcp socket bound wrong parameter");
    ret = zmtp_socket_bind(sock, NULL, &port_be);
    fail_if(ret == 0, "socket> tcp socket bound wrong parameter");
    ret = zmtp_socket_bind(sock, &inaddr_link, NULL);
    fail_if(ret == 0, "socket> tcp socket bound wrong parameter");
    /* socket_bind passing correct parameters */
    ret = zmtp_socket_bind(sock, &inaddr_link, &port_be);
    fail_if(ret < 0, "socket> tcp socket bind failed");
    //pico_free(sock);
    
  }
  
}
void test_zmtp_socket_open()
{
  struct zmtp_socket * sock;
  //test invalid arguments
  printf("test zmtp socket\n\n");
  sock = zmtp_socket_open(5, PICO_PROTO_TCP, ZMQ_TYPE_PUBLISHER, &empty_cb);
  fail_if(sock != NULL, "test_zmtp_socket failed on invalid first argument");
  sock = zmtp_socket_open(PICO_PROTO_IPV4, NULL, ZMQ_TYPE_SUBSCRIBER, &empty_cb);
  fail_if(sock != NULL, "test_zmtp_socket should fail on nulled protocol");
  sock = zmtp_socket_open(PICO_PROTO_IPV4, PICO_PROTO_TCP, NULL, &empty_cb);
  fail_if(sock != NULL, "test_zmtp_socket should fail on nulled type");
  sock = zmtp_socket_open(PICO_PROTO_IPV4, PICO_PROTO_TCP, ZMQ_TYPE_PUBLISHER, NULL );
  fail_if(sock != NULL, "test_zmtp_socket should fail on null wakeup");

  printf("test valid");
  //test valid arguments
  sock = zmtp_socket_open(PICO_PROTO_IPV4, PICO_PROTO_TCP, ZMQ_TYPE_SUBSCRIBER, &empty_cb);
  fail_if(sock == NULL, "test_zmtp_socket failed on valid arguments");
}

void test_zmtp_socket_connect()
{
    struct zmtp_socket* sock;
    uint16_t port_be;
    int8_t ret;
    struct pico_ip4 inaddr_dst;

    pico_string_to_ipv4("10.10.10.10", &inaddr_dst.addr);
    port_be = short_be(5555);

    ret = zmtp_socket_connect(NULL, &inaddr_dst, port_be);
    fail_if(ret == 0, "test zmtp socket connect failed on invalid socket argument");
    ret = zmtp_socket_connect(sock, NULL, port_be);
    fail_if(ret == 0, "test zmtp socket connect failed on invalid server address");
    ret = zmtp_socket_connect(sock, &inaddr_dst, 0);
    fail_if(ret == 0, "test zmtp socket connect failed on invalid port (0)");
    //TODO: invalid port (max_port)

    //TODO: Are these arguments correct?
    //Or, can we mock the pico_socket_connect that will be called by zmtp_socket_connect
    ret = zmtp_socket_connect(sock, &inaddr_dst, port_be);
    fail_if(ret < 0, "Error socket connect");
}

