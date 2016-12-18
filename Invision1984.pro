TEMPLATE = subdirs

CONFIG += ordered
SUBDIRS += \
    VideoCapture \
    1984    \

1984.depends += VideoCapture
