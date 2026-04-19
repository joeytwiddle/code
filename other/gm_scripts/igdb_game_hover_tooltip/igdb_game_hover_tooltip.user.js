// ==UserScript==
// @name         IGDB game hover tooltip
// @namespace    igdb-game-hover-tooltip
// @version      1.4.2
// @description  On game sites, hover over a game to display a tooltip with the game's rating, summary, and related info
// @license      ISC
// @match        *://*.humblebundle.com/*
// @match        *://*.steampowered.com/*
// @match        *://*.store.steampowered.com/*
// @match        *://*.gog.com/*
// @match        *://*.epicgames.com/*
// @match        *://*.nintendo.com/*
// @match        *://*.playstation.com/*
// @match        *://*.xbox.com/*
// @match        *://*.metacritic.com/*
// @match        *://*.opencritic.com/*
// @match        *://*.ign.com/*
// @match        *://*.gamespot.com/*
// @match        *://*.pcgamer.com/*
// @match        *://*.rockpapershotgun.com/*
// @match        *://*.polygon.com/*
// @match        *://*.kotaku.com/*
// @match        *://*.gamefaqs.gamespot.com/*
// @match        *://*.giantbomb.com/*
// @match        *://*.pcgamingwiki.com/*
// @match        *://*.protondb.com/*
// @match        *://*.lutris.net/*
// @match        *://*.itch.io/*
// @match        *://*.indiegameplus.com/*
// @match        *://*.gamepass.com/*
// @match        *://*.ubisoft.com/*
// @match        *://*.ea.com/*
// @match        *://*.battle.net/*
// @match        *://*.greenmangaming.com/*
// @match        *://*.fanatical.com/*
// @match        *://*.humblegames.com/*
// @match        *://*.g2a.com/*
// @match        *://*.kinguin.net/*
// @match        *://*.gamersgate.com/*
// @match        *://*.gamesplanet.com/*
// @match        *://*.bundlestars.com/*
// @match        *://*.chrono.gg/*
// @match        *://*.indiegala.com/*
// @match        *://*.steamdb.info/*
// @match        *://*.isthereanydeal.com/*
// @match        *://*.gg.deals/*
// @match        *://*.cheapshark.com/*
// @match        *://*.bargainbin.org/*
// @match        *://*.gamesradar.com/*
// @match        *://*.eurogamer.net/*
// @match        *://*.vg247.com/*
// @match        *://*.destructoid.com/*
// @match        *://*.pcmag.com/*
// @match        *://*.techradar.com/*
// @match        *://*.tomshardware.com/*
// @match        *://*.windowscentral.com/*
// @match        *://*.androidcentral.com/*
// @match        *://*.imdb.com/*
// @match        *://*.rawg.io/*
// @match        *://*.howlongtobeat.com/*
// @match        *://*.steamcommunity.com/*
// @match        *://*.reddit.com/r/gaming/*
// @match        *://*.gamerankings.com/*
// @match        *://*.neogaf.com/*
// @match        *://*.resetera.com/*
// @match        *://*.gameinformer.com/*
// @match        *://*.gamesindustry.biz/*
// @match        *://*.venturebeat.com/*
// @match        *://*.arstechnica.com/*
// @match        *://*.wired.com/*
// @match        *://*.theverge.com/*
// @match        *://*.engadget.com/*
// @match        *://*.digitaltrends.com/*
// @match        *://*.makeuseof.com/*
// @match        *://*.lifewire.com/*
// @match        *://*.howtogeek.com/*
// @match        *://*.pcgamesn.com/*
// @match        *://*.gameskinny.com/*
// @match        *://*.segmentnext.com/*
// @match        *://*.twinfinite.net/*
// @match        *://*.gamingbolt.com/*
// @match        *://*.shacknews.com/*
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

	// Twitch / IGDB API credentials
	// If this doesn't work for you, get your own account here: https://api-docs.igdb.com/#account-creation
	var CLIENT_ID = 'yykw3oww4um4fq81z5dfpmae5qwj4n';
	var CLIENT_SECRET = 'sbq2hgahyqcyvrmlzeh70jqbg7ln3j';

	var TOKEN_KEY = 'igdb_gm_hover_token_v1';
	var CACHE_PREFIX = 'igdb_gm_hover_cache_v2:';
	var HOVER_DELAY_MS = 180;
	/** Time to leave the source and reach the tooltip before it closes (crossing empty space). */
	var CLOSE_GRACE_MS = 180;
	var SUMMARY_MAX_CHARS = 420;

	var hoverState = {
		root: null,
		title: null,
		/** Same game = same .content-choice node (Humble-style) or same game <a>. */
		activeContentChoice: null,
		activeAnchor: null,
		enterTimer: null,
		closeTimer: null,
		seq: 0,
	};

	var tooltipEl = null;
	var cacheMem = Object.create(null);
	var pointerTrackingBound = false;

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
			.trim()
			// For Steam thumbnails
			.replace(/'s screenshot [0-9]*/, '');
	}

	var isHumbleBundle = /humblebundle\.com/i.test(location.hostname);

	/**
	 * Humble-style choice cards: innermost .content-choice, then .content-choice-title
	 * (avoids a parent wrapper that contains several titles returning the wrong game).
	 * Otherwise walk up for legacy markup; then nearest anchor text / title="".
	 */
	function getGameTitleFromElement(start) {
		if (isHumbleBundle) {
			var cc = start.closest && start.closest('.content-choice');
			if (cc) {
				var ct0 = cc.querySelector('.content-choice-title');
				if (ct0) {
					var t0 = normalizeTitleText(ct0.textContent || '');
					if (t0) return t0;
				}
			}
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
		// Steam store
		const imgElem = a && a.querySelector('img[alt]');
		if (imgElem && imgElem.alt) return normalizeTitleText(imgElem.alt);
		//
		if (a && a.tagName === 'A') {
			var linkText = normalizeTitleText(a.textContent || '');
			if (linkText && linkText.length <= 220 && !/^https?:\/\//i.test(linkText)) return linkText;
			var ta = a.getAttribute('title');
			if (ta && normalizeTitleText(ta)) return normalizeTitleText(ta);
		}
		return null;
	}

	function findHoverRoot(start, title) {
		if (isHumbleBundle) {
			var cc = start.closest && start.closest('.content-choice');
			if (cc && cc.querySelector('.content-choice-title')) return cc;
		}
		var a = start.closest && start.closest('a');
		if (a && normalizeTitleText(getGameTitleFromElement(a) || '') === normalizeTitleText(title || '')) {
			return a;
		}
		return start;
	}

	function setActiveGameIdentityFromRoot(root) {
		hoverState.activeContentChoice = null;
		hoverState.activeAnchor = null;
		if (!root || root.nodeType !== 1) return;
		if (isHumbleBundle) {
			var cc =
				root.matches && root.matches('.content-choice')
					? root
					: root.closest && root.closest('.content-choice');
			if (cc) {
				hoverState.activeContentChoice = cc;
				return;
			}
		}
		var a =
			root.matches && root.matches('a')
				? root
				: root.closest && root.closest('a');
		if (a) hoverState.activeAnchor = a;
	}

	/** Same physical game tile: exact .content-choice or exact <a> we armed from. */
	function pointerStillOnSameGame(node) {
		if (!node || node.nodeType !== 1) return false;
		if (isHumbleBundle && hoverState.activeContentChoice) {
			var cc = node.closest && node.closest('.content-choice');
			return !!cc && cc === hoverState.activeContentChoice;
		}
		if (hoverState.activeAnchor) {
			var a = node.closest && node.closest('a');
			return !!a && a === hoverState.activeAnchor;
		}
		if (!hoverState.root || hoverState.title == null || hoverState.title === '') return false;
		if (!hoverState.root.contains(node)) return false;
		var t = getGameTitleFromElement(node);
		if (!t) return false;
		return normalizeTitleText(t) === normalizeTitleText(hoverState.title);
	}

	/** True when the pointer is on a different choice card or different game link. */
	function isDifferentGameTarget(node) {
		if (!node || node.nodeType !== 1) return false;
		if (isHumbleBundle && hoverState.activeContentChoice) {
			var cc = node.closest && node.closest('.content-choice');
			return !!cc && cc !== hoverState.activeContentChoice;
		}
		if (hoverState.activeAnchor) {
			var a = node.closest && node.closest('a');
			return !!a && a !== hoverState.activeAnchor;
		}
		var t = getGameTitleFromElement(node);
		return !!(
			t &&
			hoverState.title &&
			normalizeTitleText(t) !== normalizeTitleText(hoverState.title)
		);
	}

	function clearTimers() {
		if (hoverState.enterTimer) {
			clearTimeout(hoverState.enterTimer);
			hoverState.enterTimer = null;
		}
		if (hoverState.closeTimer) {
			clearTimeout(hoverState.closeTimer);
			hoverState.closeTimer = null;
		}
	}

	function bindTooltipPointerTracking() {
		if (pointerTrackingBound) return;
		document.addEventListener('mousemove', onPointerMoveWhileTooltipVisible, true);
		document.addEventListener('pointermove', onPointerMoveWhileTooltipVisible, true);
		pointerTrackingBound = true;
	}

	function unbindTooltipPointerTracking() {
		if (!pointerTrackingBound) return;
		document.removeEventListener('mousemove', onPointerMoveWhileTooltipVisible, true);
		document.removeEventListener('pointermove', onPointerMoveWhileTooltipVisible, true);
		pointerTrackingBound = false;
	}

	/** True if (x,y) is over the tooltip or still over the same .content-choice / game <a> we opened from. */
	function pointerPositionKeepsTooltipOpen(x, y) {
		if (!tooltipEl || tooltipEl.style.display === 'none') return false;
		var tip = tooltipEl.getBoundingClientRect();
		if (x >= tip.left && x <= tip.right && y >= tip.top && y <= tip.bottom) return true;
		var hit = document.elementFromPoint(x, y);
		return pointerStillOnSameGame(hit);
	}

	/**
	 * Any move off the opened source and off the tooltip should arm close — do not use
	 * getGameTitleFromElement here (ancestors often “see” unrelated game titles on the page).
	 */
	function onPointerMoveWhileTooltipVisible(ev) {
		if (!tooltipEl || tooltipEl.style.display === 'none' || !hoverState.root) return;
		var x = ev.clientX;
		var y = ev.clientY;
		if (pointerPositionKeepsTooltipOpen(x, y)) {
			cancelCloseTimer();
			return;
		}
		scheduleHideTooltip(false);
	}

	function hideTooltip() {
		clearTimers();
		unbindTooltipPointerTracking();
		hoverState.root = null;
		hoverState.title = null;
		hoverState.activeContentChoice = null;
		hoverState.activeAnchor = null;
		hoverState.seq++;
		if (tooltipEl) {
			tooltipEl.style.display = 'none';
			tooltipEl.innerHTML = '';
		}
	}

	function cancelCloseTimer() {
		if (hoverState.closeTimer) {
			clearTimeout(hoverState.closeTimer);
			hoverState.closeTimer = null;
		}
	}

	function armDelayedHover(root, title) {
		var t = title;
		var r = root;
		hoverState.root = root;
		hoverState.title = title;
		setActiveGameIdentityFromRoot(root);
		if (hoverState.enterTimer) {
			clearTimeout(hoverState.enterTimer);
			hoverState.enterTimer = null;
		}
		hoverState.enterTimer = setTimeout(function () {
			hoverState.enterTimer = null;
			if (hoverState.root !== r || hoverState.title !== t) return;
			beginHover(t);
		}, HOVER_DELAY_MS);
	}

	/**
	 * @param {boolean} [restart=true] If false and a close timer is already running, leave it (avoids
	 * extending the grace forever while scrubbing over blank UI).
	 */
	function scheduleHideTooltip(restart) {
		if (restart === false && hoverState.closeTimer != null) return;
		cancelCloseTimer();
		hoverState.closeTimer = setTimeout(function () {
			hoverState.closeTimer = null;
			hideTooltip();
			/*
			 * Do not re-arm from here: getGameTitleFromElement often matches unrelated nodes
			 * (first title under a large ancestor), which reopened the tooltip immediately.
			 * A new hover is picked up by mouseover on the next game.
			 */
		}, CLOSE_GRACE_MS);
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

	function positionTooltip(x, y) {
		if (!tooltipEl || tooltipEl.style.display === 'none') return;
		var pad = 14;
		var margin = 8;
		var vw = window.innerWidth;
		var vh = window.innerHeight;
		tooltipEl.style.left = '0';
		tooltipEl.style.top = '0';
		var rect = tooltipEl.getBoundingClientRect();
		var w = rect.width;
		var h = rect.height;
		// Past the horizontal midpoint: anchor to the left of the pointer so the panel stays on-screen.
		var left =
			x > vw / 2 ? x - w - pad : x + pad;
		var top = y + pad;
		left = Math.max(margin, Math.min(left, vw - w - margin));
		if (top + h > vh - margin) top = Math.max(margin, y - h - pad);
		tooltipEl.style.left = left + 'px';
		tooltipEl.style.top = top + 'px';
	}

	function showTooltipLoading() {
		var el = ensureTooltip();
		el.style.display = 'block';
		el.innerHTML =
			'<div class="igdb-gm-inner"><div class="igdb-gm-loading">IGDB…</div></div>';
		positionTooltip(lastPointer.x, lastPointer.y);
		bindTooltipPointerTracking();
	}

	function showTooltipHtml(html) {
		var el = ensureTooltip();
		el.style.display = 'block';
		el.innerHTML = html;
		positionTooltip(lastPointer.x, lastPointer.y);
		bindTooltipPointerTracking();
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
			'involved_companies.company.name,involved_companies.developer,involved_companies.publisher,' +
			'external_games.uid,external_games.url,external_games.external_game_source.name;' +
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

	function pickExternalBySourceName(list, sourceName) {
		if (!list || !list.length) return null;
		var i, eg;
		for (i = 0; i < list.length; i++) {
			eg = list[i];
			if (eg.external_game_source && eg.external_game_source.name === sourceName) return eg;
		}
		return null;
	}

	function outboundLink(label, href) {
		return (
			'<a class="igdb-gm-link" href="' +
			escapeHtml(href) +
			'" target="_blank" rel="noopener noreferrer">' +
			escapeHtml(label) +
			'</a>'
		);
	}

	/**
	 * Outbound searches / stores to judge whether to install (YouTube, aggregators, Steam reviews, Linux).
	 */
	function buildReviewLinksHtml(displayName, externalGames) {
		var name = displayName || '';
		var chunks = [];

		var mc =
			'https://www.metacritic.com/search/' + encodeURIComponent(name) + '/?category=game';
		chunks.push(outboundLink('Metacritic search', mc));

		var oc =
			'https://duckduckgo.com/?q=' +
			encodeURIComponent(name + ' game site:opencritic.com');
		chunks.push(outboundLink('OpenCritic (via DDG)', oc));

		var proton = 'https://www.protondb.com/search?q=' + encodeURIComponent(name);
		chunks.push(outboundLink('ProtonDB (Linux)', proton));

		var steam = pickExternalBySourceName(externalGames, 'Steam');
		if (steam) {
			var steamDigits = String(steam.uid || '').replace(/\D/g, '');
			var storeUrl =
				steam.url ||
				(steamDigits ? 'https://store.steampowered.com/app/' + steamDigits + '/' : '');
			if (storeUrl) {
				chunks.unshift(outboundLink('Steam store', storeUrl));
				if (steamDigits) {
					chunks.splice(
						1,
						0,
						outboundLink(
							'Steam user reviews',
							'https://steamcommunity.com/app/' + steamDigits + '/reviews/'
						)
					);
				}
			}
		}

		var gog = pickExternalBySourceName(externalGames, 'GOG');
		if (gog && gog.url) chunks.push(outboundLink('GOG store', gog.url));

		var epic = pickExternalBySourceName(externalGames, 'Epic Game Store');
		if (epic && epic.url) chunks.push(outboundLink('Epic store', epic.url));

		var ytReviews =
			'https://www.youtube.com/results?search_query=' +
			encodeURIComponent('review game ' + name);
		chunks.push(outboundLink('YouTube: review search', ytReviews));

		var ytBuy =
			'https://www.youtube.com/results?search_query=' +
			encodeURIComponent(name + ' before you buy');
		chunks.push(outboundLink('YouTube: before you buy', ytBuy));

		return (
			'<div class="igdb-gm-links">' +
			'<div class="igdb-gm-links-h">Decide to install</div>' +
			'<div class="igdb-gm-linkrow">' +
			chunks.join('<span class="igdb-gm-sep"> · </span>') +
			'</div></div>'
		);
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
		var genres = (game.genres || []).map(g => g.name).filter(Boolean);
		var plats = (game.platforms || []).map(p => p.name).filter(Boolean);
		var modes = (game.game_modes || []).map(m => m.name).filter(Boolean);

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
				  ratings.map((r) => '<div>' + escapeHtml(r) + '</div>').join('') +
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
			buildReviewLinksHtml(game.name || '', game.external_games) +
			'<div class="igdb-gm-footer">' +
			igdbLink +
			'<span class="igdb-gm-note">IGDB summary is editorial blurbs; scores blend critics/users.</span>' +
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

		if (tooltipEl && tooltipEl.style.display !== 'none') {
			if (tooltipEl.contains(ev.target)) {
				cancelCloseTimer();
				return;
			}
			if (pointerStillOnSameGame(ev.target)) cancelCloseTimer();
		}

		var title = getGameTitleFromElement(ev.target);
		if (!title) {
			if (
				tooltipEl &&
				tooltipEl.style.display !== 'none' &&
				!tooltipEl.contains(ev.target) &&
				!pointerStillOnSameGame(ev.target)
			) {
				scheduleHideTooltip(false);
			}
			return;
		}
		var root = findHoverRoot(ev.target, title);
		if (hoverState.root === root) return;
		/*
		 * Grace: ignore other games while the close timer runs — unless the pointer is on a
		 * different .content-choice or different game <a>, then switch immediately.
		 */
		if (
			tooltipEl &&
			tooltipEl.style.display !== 'none' &&
			hoverState.closeTimer != null &&
			!isDifferentGameTarget(ev.target)
		) {
			return;
		}
		hideTooltip();
		armDelayedHover(root, title);
	}

	function onDocumentMouseOut(ev) {
		if (!tooltipEl || tooltipEl.style.display === 'none') return;
		var to = ev.relatedTarget;
		if (to) {
			if (tooltipEl.contains(to)) return;
			if (pointerStillOnSameGame(to)) return;
		}
		scheduleHideTooltip();
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
			background: rgba(20, 22, 28, 1);
			border: 1px solid rgba(255,255,255,0.12);
			border-radius: 10px;
			box-shadow: 0 12px 40px rgba(0,0,0,0.45);
			pointer-events: auto;
		}
		#igdb-gm-hover-tooltip a { color: #8cb4ff; }
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
		.igdb-gm-links { margin-top: 10px; padding-top: 8px; border-top: 1px solid rgba(255,255,255,0.08); }
		.igdb-gm-links-h { font-size: 11px; font-weight: 600; color: #bbb; margin-bottom: 6px; }
		.igdb-gm-linkrow { font-size: 11px; line-height: 1.55; color: #999; }
		.igdb-gm-sep { color: #555; user-select: none; }
		.igdb-gm-footer { margin-top: 10px; display: flex; flex-wrap: wrap; align-items: center; gap: 8px; font-size: 10px; color: #666; }
		.igdb-gm-loading { padding: 20px 28px; color: #aaa; }
		.igdb-gm-error { display: block; padding: 12px 14px; color: #f88; }
		.igdb-gm-error p { margin: 8px 0 0; color: #ccc; font-size: 12px; }
	`);

	document.addEventListener('mouseover', onDocumentMouseOver, true);
	document.addEventListener('mouseout', onDocumentMouseOut, true);
})();
