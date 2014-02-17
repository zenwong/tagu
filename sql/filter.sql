select
vids.title as title,
vids.path as path
from vids
inner join vidtags on vidtags.title = vids.title
and vidtags.tid in(41, 42)
group by vids.title having count(vids.title) = 2

intersect

select
vids.title as title,
vids.path as path
from vids
inner join vidacts on vidacts.title = vids.title
and vidacts.aid in(36, 37)
group by vids.title having count(vids.title) = 2


;
