#ifndef __SSH_AUTO_KEYGEN_H__
#define __SSH_AUTO_KEYGEN_H__

void do_gen_all_hostkeys();
int generate_public_private_keys(struct sshbuf *b);

#endif
