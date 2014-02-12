select
vids.title as title,
vids.path as path,
tags.name as tag
from vids
join tags on vidtags.tid = tags._id
join vidtags on vidtags.title = vids.title
and tag in('rape', 'incest')
group by vids.title having count(vids.title) = 2;
