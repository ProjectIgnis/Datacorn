lupdate @ts_list.txt -no-ui-lines -source-language en -target-language es -ts es.ts
lrelease es.ts -qm res/es.qm
touch res/res.qrc
