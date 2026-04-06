// ==UserScript==
// @name         IGDB game hover tooltip
// @namespace    local.igdb-game-hover-tooltip
// @version      1.0.0
// @description  On hover, look up a game title via IGDB and show rating, summary, and related info in a tooltip (Humble choice cards and plain links supported).
// @license      ISC
// @match        *://*/*
// @grant        GM_xmlhttpRequest
// @grant        GM_setValue
// @grant        GM_getValue
// @grant        GM_addStyle
// @connect      id.twitch.tv
// @connect      api.igdb.com
// @connect      images.igdb.com
// ==/UserScript==

(function () {
	'use strict';

	/** Twitch / IGDB app credentials — rotate in the dev console if this file is ever shared or committed publicly. */
	// If this doesn't work for you, get your own account here: https://api-docs.igdb.com/#account-creation
	var CLIENT_ID = 'yykw3oww4um4fq81z5dfpmae5qwj4n';
	var CLIENT_SECRET = 'sbq2hgahyqcyvrmlzeh70jqbg7ln3j';

	var TOKEN_KEY = 'igdb_gm_hover_token_v1';
	var CACHE_PREFIX = 'igdb_gm_hover_cache:';
	var HOVER_DELAY_MS = 180;
	var SUMMARY_MAX_CHARS = 420;

	var hoverState = {
		root: null,
		title: null,
		enterTimer: null,
		seq: 0,
	};

	var tooltipEl = null;
	var cacheMem = Object.create(null);

	function escapeHtml(s) {
		if (s == null || s === '') return '';
		var d = document.createElement('div');
		d.textContent = String(s);
		return d.innerHTML;
	}

	function normalizeTitleText(s) {
		return String(s)
			.replace(/\s+/g, ' ')
			.replace(/[\u2018\u2019]/g, "'")
			.trim();
	}

	/**
	 * Humble-style choice cards: .content-choice-title
	 * Otherwise nearest anchor text or its title="" attribute.
	 */
	function getGameTitleFromElement(start) {
		var el = start;
		var i;
		for (i = 0; i < 12 && el; i++, el = el.parentElement) {
			if (!el || !el.querySelector) continue;
			var ct = el.querySelector('.content-choice-title');
			if (ct) {
				var t = normalizeTitleText(ct.textContent || '');
				if (t) return t;
			}
		}
		var a = start.closest && start.closest('a');
		if (!a) {
			for (el = start; el && el !== document.documentElement; el = el.parentElement) {
				if (el.tagName === 'A') {
					a = el;
					break;
				}
			}
		}
		if (a && a.tagName === 'A') {
			var linkText = normalizeTitleText(a.textContent || '');
			if (linkText && linkText.length <= 220 && !/^https?:\/\//i.test(linkText)) return linkText;
			var ta = a.getAttribute('title');
			if (ta && normalizeTitleText(ta)) return normalizeTitleText(ta);
		}
		return null;
	}

	function findHoverRoot(start, title) {
		var cc = start.closest && start.closest('.content-choice');
		if (cc && cc.querySelector('.content-choice-title')) return cc;
		var a = start.closest && start.closest('a');
		if (a && getGameTitleFromElement(a) === title) return a;
		return start;
	}

	function clearTimers() {
		if (hoverState.enterTimer) {
			clearTimeout(hoverState.enterTimer);
			hoverState.enterTimer = null;
		}
	}

	function hideTooltip() {
		clearTimers();
		hoverState.root = null;
		hoverState.title = null;
		hoverState.seq++;
		if (tooltipEl) {
			tooltipEl.style.display = 'none';
			tooltipEl.innerHTML = '';
		}
		document.removeEventListener('mousemove', onMouseMove, true);
	}

	function ensureTooltip() {
		if (tooltipEl) return tooltipEl;
		tooltipEl = document.createElement('div');
		tooltipEl.id = 'igdb-gm-hover-tooltip';
		tooltipEl.setAttribute('role', 'tooltip');
		document.body.appendChild(tooltipEl);
		return tooltipEl;
	}

	var lastPointer = { x: 0, y: 0 };

	function onMouseMove(ev) {
		lastPointer.x = ev.clientX;
		lastPointer.y = ev.clientY;
		positionTooltip(lastPointer.x, lastPointer.y);
	}

	function positionTooltip(x, y) {
		if (!tooltipEl || tooltipEl.style.display === 'none') return;
		var pad = 14;
		var vw = window.innerWidth;
		var vh = window.innerHeight;
		tooltipEl.style.left = '0';
		tooltipEl.style.top = '0';
		var rect = tooltipEl.getBoundingClientRect();
		var w = rect.width;
		var h = rect.height;
		var left = x + pad;
		var top = y + pad;
		if (left + w > vw - 8) left = Math.max(8, vw - w - 8);
		if (top + h > vh - 8) top = Math.max(8, y - h - pad);
		tooltipEl.style.left = left + 'px';
		tooltipEl.style.top = top + 'px';
	}

	function showTooltipLoading() {
		var el = ensureTooltip();
		el.style.display = 'block';
		el.innerHTML =
			'<div class="igdb-gm-inner"><div class="igdb-gm-loading">IGDB…</div></div>';
		positionTooltip(lastPointer.x, lastPointer.y);
		document.addEventListener('mousemove', onMouseMove, true);
	}

	function showTooltipHtml(html) {
		var el = ensureTooltip();
		el.style.display = 'block';
		el.innerHTML = html;
		positionTooltip(lastPointer.x, lastPointer.y);
		document.addEventListener('mousemove', onMouseMove, true);
	}

	function getAccessToken(cb) {
		var raw = GM_getValue(TOKEN_KEY);
		if (raw) {
			try {
				var c = JSON.parse(raw);
				if (c.expiresAt > Date.now() + 10000) return cb(null, c.accessToken);
			} catch (e) {}
		}
		GM_xmlhttpRequest({
			method: 'POST',
			url: 'https://id.twitch.tv/oauth2/token',
			headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
			data:
				'client_id=' +
				encodeURIComponent(CLIENT_ID) +
				'&client_secret=' +
				encodeURIComponent(CLIENT_SECRET) +
				'&grant_type=client_credentials',
			onload: function (res) {
				if (res.status !== 200) return cb(new Error('OAuth HTTP ' + res.status));
				var j = JSON.parse(res.responseText);
				var expiresAt = Date.now() + j.expires_in * 1000 - 120000;
				GM_setValue(
					TOKEN_KEY,
					JSON.stringify({ accessToken: j.access_token, expiresAt: expiresAt })
				);
				cb(null, j.access_token);
			},
			onerror: function () {
				cb(new Error('OAuth network error'));
			},
		});
	}

	function igdbPost(path, body, cb) {
		getAccessToken(function (err, token) {
			if (err) return cb(err);
			GM_xmlhttpRequest({
				method: 'POST',
				url: 'https://api.igdb.com/v4' + path,
				headers: {
					'Client-ID': CLIENT_ID,
					Authorization: 'Bearer ' + token,
					Accept: 'application/json',
					'Content-Type': 'text/plain',
				},
				data: body,
				onload: function (res) {
					if (res.status !== 200) {
						return cb(
							new Error('IGDB ' + res.status + ': ' + (res.responseText || '').slice(0, 240))
						);
					}
					try {
						cb(null, JSON.parse(res.responseText));
					} catch (e) {
						cb(e);
					}
				},
				onerror: function () {
					cb(new Error('IGDB network error'));
				},
			});
		});
	}

	function formatRating(label, value, count) {
		if (value == null || value === '') return '';
		var n = typeof value === 'number' ? Math.round(value * 10) / 10 : value;
		var line = label + ': ' + n + '/100';
		if (count != null && count > 0) line += ' (' + count + ' votes)';
		return line;
	}

	function formatDate(ts) {
		if (ts == null) return '';
		var d = new Date(ts * 1000);
		if (isNaN(d.getTime())) return '';
		return d.toLocaleDateString(undefined, { year: 'numeric', month: 'short', day: 'numeric' });
	}

	function truncate(s, n) {
		s = String(s);
		if (s.length <= n) return s;
		return s.slice(0, n - 1).trim() + '…';
	}

	function coverUrl(cover) {
		if (!cover || !cover.url) return '';
		var u = cover.url;
		if (u.indexOf('//') === 0) return 'https:' + u;
		return u;
	}

	function buildGameBody(title) {
		var q = title.replace(/"/g, '').replace(/\r|\n/g, ' ').trim();
		return (
			'search "' +
			q +
			'"; fields name,slug,summary,storyline,first_release_date,' +
			'aggregated_rating,aggregated_rating_count,total_rating,total_rating_count,' +
			'url,cover.url,genres.name,platforms.name,game_modes.name,' +
			'involved_companies.company.name,involved_companies.developer,involved_companies.publisher;' +
			' limit 1;'
		);
	}

	function pickInvolved(list, role) {
		if (!list || !list.length) return '';
		for (var i = 0; i < list.length; i++) {
			var ic = list[i];
			if (ic[role] && ic.company && ic.company.name) return ic.company.name;
		}
		return '';
	}

	function cacheGet(key) {
		if (cacheMem[key] !== undefined) return cacheMem[key];
		var raw = GM_getValue(CACHE_PREFIX + key);
		if (!raw) return null;
		try {
			var o = JSON.parse(raw);
			if (o.exp && o.exp < Date.now()) {
				return null;
			}
			cacheMem[key] = o.data;
			return o.data;
		} catch (e) {
			return null;
		}
	}

	function cacheSet(key, data) {
		cacheMem[key] = data;
		var exp = Date.now() + 86400000 * 3;
		GM_setValue(CACHE_PREFIX + key, JSON.stringify({ exp: exp, data: data }));
	}

	function renderGameHtml(game, ttb) {
		var cover = coverUrl(game.cover);
		var genres = (game.genres || []).map(function (g) { return g.name; }).filter(Boolean);
		var plats = (game.platforms || []).map(function (p) { return p.name; }).filter(Boolean);
		var modes = (game.game_modes || []).map(function (m) { return m.name; }).filter(Boolean);

		var dev = pickInvolved(game.involved_companies, 'developer');
		var pub = pickInvolved(game.involved_companies, 'publisher');

		var ratings = [];
		var ar = formatRating(
			'Critics (aggregated)',
			game.aggregated_rating,
			game.aggregated_rating_count
		);
		if (ar) ratings.push(ar);
		var tr = formatRating('Users (total)', game.total_rating, game.total_rating_count);
		if (tr) ratings.push(tr);

		var blurb = game.summary || game.storyline || '';
		if (blurb) blurb = truncate(blurb, SUMMARY_MAX_CHARS);

		var ttbLine = '';
		if (ttb && ttb.normally) {
			var hrs = Math.round((ttb.normally / 3600) * 10) / 10;
			ttbLine =
				'<div class="igdb-gm-ttb">Typical time to beat (HowLongToBeat via IGDB): ~' +
				hrs +
				' h</div>';
		}

		var igdbLink = game.url
			? '<a class="igdb-gm-link" href="' +
			  escapeHtml(game.url) +
			  '" target="_blank" rel="noopener noreferrer">Open on IGDB</a>'
			: '';

		return (
			'<div class="igdb-gm-inner">' +
			(cover
				? '<img class="igdb-gm-cover" src="' +
				  escapeHtml(cover) +
				  '" width="120" height="auto" alt="" />'
				: '') +
			'<div class="igdb-gm-main">' +
			'<div class="igdb-gm-title">' +
			escapeHtml(game.name || '') +
			'</div>' +
			(ratings.length
				? '<div class="igdb-gm-ratings">' +
				  ratings.map(function (r) { return '<div>' + escapeHtml(r) + '</div>'; }).join('') +
				  '</div>'
				: '<div class="igdb-gm-muted">No IGDB score yet.</div>') +
			'<div class="igdb-gm-meta">' +
			(game.first_release_date
				? '<span>Released: ' + escapeHtml(formatDate(game.first_release_date)) + '</span>'
				: '') +
			(genres.length ? '<span>' + escapeHtml(genres.slice(0, 4).join(', ')) + '</span>' : '') +
			'</div>' +
			(plats.length
				? '<div class="igdb-gm-platforms">' + escapeHtml(plats.slice(0, 8).join(' · ')) + '</div>'
				: '') +
			(modes.length
				? '<div class="igdb-gm-muted">' + escapeHtml(modes.join(', ')) + '</div>'
				: '') +
			(dev ? '<div class="igdb-gm-dev">Developer: ' + escapeHtml(dev) + '</div>' : '') +
			(pub && pub !== dev
				? '<div class="igdb-gm-pub">Publisher: ' + escapeHtml(pub) + '</div>'
				: '') +
			ttbLine +
			(blurb
				? '<div class="igdb-gm-summary"><strong>Summary</strong><p>' +
				  escapeHtml(blurb) +
				  '</p></div>'
				: '') +
			'<div class="igdb-gm-footer">' +
			igdbLink +
			'<span class="igdb-gm-note">Summary text is IGDB’s own blurb (not a press review).</span>' +
			'</div>' +
			'</div></div>'
		);
	}

	function fetchGameBundle(title, mySeq, done) {
		var ck = cacheGet(title);
		if (ck) {
			if (mySeq !== hoverState.seq) return;
			return done(null, ck.game, ck.ttb);
		}

		igdbPost('/games', buildGameBody(title), function (err, games) {
			if (mySeq !== hoverState.seq) return;
			if (err) return done(err);
			if (!games || !games.length) return done(new Error('No match'));

			var game = games[0];
			igdbPost(
				'/game_time_to_beats',
				'where game_id = ' +
					game.id +
					'; fields hastily,normally,completely,count; limit 1;',
				function (err2, rows) {
					if (mySeq !== hoverState.seq) return;
					var ttb = !err2 && rows && rows[0] ? rows[0] : null;
					cacheSet(title, { game: game, ttb: ttb });
					done(null, game, ttb);
				}
			);
		});
	}

	function beginHover(title) {
		var mySeq = ++hoverState.seq;
		showTooltipLoading();
		fetchGameBundle(title, mySeq, function (err, game, ttb) {
			if (mySeq !== hoverState.seq) return;
			if (err) {
				showTooltipHtml(
					'<div class="igdb-gm-inner igdb-gm-error"><strong>' +
						escapeHtml(title) +
						'</strong><p>' +
						escapeHtml(err.message || String(err)) +
						'</p></div>'
				);
				return;
			}
			showTooltipHtml(renderGameHtml(game, ttb));
		});
	}

	function onDocumentMouseOver(ev) {
		lastPointer.x = ev.clientX;
		lastPointer.y = ev.clientY;
		var title = getGameTitleFromElement(ev.target);
		if (!title) return;
		var root = findHoverRoot(ev.target, title);
		if (hoverState.root === root) return;
		hideTooltip();
		hoverState.root = root;
		hoverState.title = title;
		var t = title;
		hoverState.enterTimer = setTimeout(function () {
			hoverState.enterTimer = null;
			if (hoverState.root !== root || hoverState.title !== t) return;
			beginHover(t);
		}, HOVER_DELAY_MS);
	}

	function onDocumentMouseOut(ev) {
		if (!hoverState.root) return;
		var to = ev.relatedTarget;
		if (to && hoverState.root.contains(to)) return;
		hideTooltip();
	}

	GM_addStyle(`
#igdb-gm-hover-tooltip {
	position: fixed;
	z-index: 2147483647;
	max-width: 420px;
	max-height: min(78vh, 640px);
	overflow: auto;
	padding: 0;
	margin: 0;
	font: 13px/1.45 system-ui, -apple-system, Segoe UI, Roboto, Ubuntu, sans-serif;
	color: #eee;
	background: rgba(20, 22, 28, 0.96);
	border: 1px solid rgba(255,255,255,0.12);
	border-radius: 10px;
	box-shadow: 0 12px 40px rgba(0,0,0,0.45);
	pointer-events: none;
}
#igdb-gm-hover-tooltip a { pointer-events: auto; color: #8cb4ff; }
.igdb-gm-inner { display: flex; gap: 12px; padding: 12px 14px; align-items: flex-start; }
.igdb-gm-cover {
	flex: 0 0 auto;
	border-radius: 6px;
	object-fit: cover;
	max-height: 160px;
}
.igdb-gm-main { flex: 1; min-width: 0; }
.igdb-gm-title { font-weight: 700; font-size: 15px; margin-bottom: 6px; color: #fff; }
.igdb-gm-ratings { margin-bottom: 6px; color: #b8d487; font-size: 12px; }
.igdb-gm-meta { font-size: 11px; color: #aaa; display: flex; flex-wrap: wrap; gap: 6px 12px; margin-bottom: 4px; }
.igdb-gm-platforms { font-size: 11px; color: #ccc; margin-bottom: 4px; }
.igdb-gm-muted { font-size: 11px; color: #888; margin-bottom: 4px; }
.igdb-gm-dev, .igdb-gm-pub { font-size: 11px; color: #bbb; }
.igdb-gm-summary { margin-top: 8px; font-size: 12px; color: #ddd; }
.igdb-gm-summary p { margin: 4px 0 0; }
.igdb-gm-ttb { font-size: 11px; color: #9cdcfe; margin-top: 6px; }
.igdb-gm-footer { margin-top: 10px; display: flex; flex-wrap: wrap; align-items: center; gap: 8px; font-size: 10px; color: #666; }
.igdb-gm-loading { padding: 20px 28px; color: #aaa; }
.igdb-gm-error { display: block; padding: 12px 14px; color: #f88; }
.igdb-gm-error p { margin: 8px 0 0; color: #ccc; font-size: 12px; }
`);

	document.addEventListener('mouseover', onDocumentMouseOver, true);
	document.addEventListener('mouseout', onDocumentMouseOut, true);
})();
