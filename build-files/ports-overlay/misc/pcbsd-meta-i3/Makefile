# New ports collection makefile for:	pcbsd-meta-i3
#
# $FreeBSD$
#

PORTNAME=	pcbsd-meta-i3
PORTVERSION=	CHGVERSION
CATEGORIES=	misc
MASTER_SITES=	# empty
DISTFILES=	# none

MAINTAINER=	kris@pcbsd.org
COMMENT=	The meta-port pcbsd-meta-i3

RUN_DEPENDS=	i3>=0:${PORTSDIR}/x11-wm/i3 \
		i3lock>=0:${PORTSDIR}/x11/i3lock \
		i3status>=0:${PORTSDIR}/x11/i3status

NO_BUILD=	yes

do-install:
	${DO_NADA}

.include <bsd.port.mk>
