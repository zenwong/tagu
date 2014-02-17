select
vids.title as title,
vids.path as path
from vids
inner join vidtags on vidtags.title = vids.title
inner join tags on tags._id = vidtags.tid
where tags.name = ?
union

select
vids.title as title,
vids.path as path
from vids
inner join vidacts on vidacts.title = vids.title
inner join acts on acts._id = vidacts.aid
where acts.name = ?
