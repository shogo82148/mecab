FROM buildpack-deps:buster AS builder

RUN apt-get update
RUN apt-get install -y --no-install-recommends gettext

COPY mecab /mecab
COPY mecab-jumandic /mecab-jumandic

WORKDIR /mecab
RUN ./autogen.sh
RUN ./configure --enable-utf8-only
RUN make -j$(nproc)
RUN make install
RUN ldconfig

WORKDIR /mecab-jumandic
RUN ./autogen.sh
RUN ./configure --with-charset=utf8
RUN make
RUN make install

FROM debian:buster-slim
COPY --from=builder /usr/local/lib/libmecab.so.2.0.1 /usr/local/lib/
RUN cd /usr/local/lib && ln -s -f libmecab.so.2.0.1 libmecab.so.2 && ln -s -f libmecab.so.2.0.1 libmecab.so
COPY --from=builder /usr/local/lib/libmecab.a /usr/local/lib/
COPY --from=builder /usr/local/bin/mecab /usr/local/bin/
COPY --from=builder /usr/local/include/mecab.h /usr/local/include/
COPY --from=builder /usr/local/lib/pkgconfig/mecab.pc /usr/local/lib/pkgconfig/
COPY --from=builder /usr/local/libexec/mecab /usr/local/libexec/
COPY --from=builder /usr/local/lib/mecab/dic/ipadic /usr/local/lib/mecab/dic/ipadic
RUN ldconfig && mkdir -p /usr/local/etc && echo dicdir = /usr/local/lib/mecab/dic/jumandic > /usr/local/etc/mecabrc
ENTRYPOINT /usr/local/bin/mecab
