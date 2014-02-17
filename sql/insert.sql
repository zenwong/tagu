insert into tags(name) values('rape');
insert into tags(name) values('teacher');
insert into tags(name) values('incest');

insert into acts(name) values('Mikuni Maisaki');
insert into acts(name) values('Kaho Kasumi');
insert into acts(name) values('Riko Tachibana');

insert into vids(title) values('RBD-555');
insert into vids(title) values('SDMT-939');
insert into vids(title) values('NHDTA-454');

insert into vidacts(vid,aid) values(1,1);
insert into vidacts(vid,aid) values(1,2);
insert into vidacts(vid,aid) values(1,3);
insert into vidacts(vid,aid) values(2,1);
insert into vidacts(vid,aid) values(2,2);
insert into vidacts(vid,aid) values(2,3);
insert into vidacts(vid,aid) values(3,1);
insert into vidacts(vid,aid) values(3,2);
insert into vidacts(vid,aid) values(3,3);

insert into vidtags(vid,tid) values(1,1);
insert into vidtags(vid,tid) values(1,2);
insert into vidtags(vid,tid) values(1,3);
insert into vidtags(vid,tid) values(2,1);
insert into vidtags(vid,tid) values(2,2);
insert into vidtags(vid,tid) values(2,3);
insert into vidtags(vid,tid) values(3,1);
insert into vidtags(vid,tid) values(3,2);
insert into vidtags(vid,tid) values(3,3);

-- populate full text search with values from library view
insert into search(docid,title,tags,acts) select vid as docid,title,tags,acts from LibraryView;
